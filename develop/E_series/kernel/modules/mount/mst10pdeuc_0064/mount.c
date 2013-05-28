/*

*/

#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/acct.h>
#include <linux/capability.h>
#include <linux/cpumask.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/seq_file.h>
#include <linux/mnt_namespace.h>
#include <linux/namei.h>
#include <linux/nsproxy.h>
#include <linux/security.h>
#include <linux/mount.h>
#include <linux/ramfs.h>
#include <linux/log2.h>
#include <linux/idr.h>
#include <linux/fs_struct.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include "pnode.h"
#include "internal.h"


struct vfsmount *(*copy_tree_api)(struct vfsmount *mnt, struct dentry *dentry,int flag);
static struct vfsmount *(*clone_mnt_api)(struct vfsmount *old, struct dentry *root,int flag);
void (*release_mounts_api)(struct list_head *head);
void (*umount_tree_api)(struct vfsmount *mnt, int propagate, struct list_head *kill);
static void (*touch_mnt_namespace_api)(struct mnt_namespace *ns);
int (*do_remount_sb_api)(struct super_block *sb, int flags, void *data, int force);
int (*attach_recursive_mnt_api)(struct vfsmount *source_mnt,struct path *path, struct path *parent_path); //C00CFE70
static int (*invent_group_ids_api)(struct vfsmount *mnt, bool recurse);
void (*change_mnt_propagation_api)(struct vfsmount *mnt, int type);

static long (*mount_orig)( char __user * dev_name, char __user * dir_name, char __user * type, unsigned long flags, void __user * data);

__cacheline_aligned_in_smp DEFINE_SPINLOCK(vfsmount_lock);


static struct rw_semaphore namespace_sem;

static unsigned int count_mnt_writers(struct vfsmount *mnt)
{
#ifdef CONFIG_SMP
	unsigned int count = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		count += *per_cpu_ptr(mnt->mnt_writers, cpu);
	}

	return count;
#else
	return mnt->mnt_writers;
#endif
}

static inline int check_mnt(struct vfsmount *mnt)
{
	return mnt->mnt_ns == current->nsproxy->mnt_ns;
}
static int graft_tree(struct vfsmount *mnt, struct path *path)
{
	int err;
	if (mnt->mnt_sb->s_flags & MS_NOUSER)
		return -EINVAL;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(mnt->mnt_root->d_inode->i_mode))
		return -ENOTDIR;

	err = -ENOENT;
	mutex_lock(&path->dentry->d_inode->i_mutex);
	if (cant_mount(path->dentry))
		goto out_unlock;

	if (!d_unlinked(path->dentry))
		err = attach_recursive_mnt_api(mnt, path, NULL);
out_unlock:
	mutex_unlock(&path->dentry->d_inode->i_mutex);
	return err;
}

static int mount_is_safe(struct path *path)
{
	if (capable(CAP_SYS_ADMIN))
		return 0;
	return -EPERM;
#ifdef notyet
	if (S_ISLNK(path->dentry->d_inode->i_mode))
		return -EPERM;
	if (path->dentry->d_inode->i_mode & S_ISVTX) {
		if (current_uid() != path->dentry->d_inode->i_uid)
			return -EPERM;
	}
	if (inode_permission(path->dentry->d_inode, MAY_WRITE))
		return -EPERM;
	return 0;
#endif
}
static struct vfsmount *next_mnt(struct vfsmount *p, struct vfsmount *root)
{
	struct list_head *next = p->mnt_mounts.next;
	if (next == &p->mnt_mounts) {
		while (1) {
			if (p == root)
				return NULL;
			next = p->mnt_child.next;
			if (next != &p->mnt_parent->mnt_mounts)
				break;
			p = p->mnt_parent;
		}
	}
	return list_entry(next, struct vfsmount, mnt_child);
}
static int do_change_type(struct path *path, int flag)
{
	struct vfsmount *m, *mnt = path->mnt;
	int recurse = flag & MS_REC;
	int type = flag & ~MS_REC;
	int err = 0;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	down_write(&namespace_sem);
	if (type == MS_SHARED) {
		err = invent_group_ids_api(mnt, recurse);
		if (err)
			goto out_unlock;
	}

	spin_lock(&vfsmount_lock);
	for (m = mnt; m; m = (recurse ? next_mnt(m, mnt) : NULL))
		change_mnt_propagation_api(m, type);
	spin_unlock(&vfsmount_lock);

 out_unlock:
	up_write(&namespace_sem);
	return err;
}

static int do_loopback(struct path *path, char *old_name,
				int recurse)
{
	struct path old_path;
	struct vfsmount *mnt = NULL;
	int err = mount_is_safe(path);
	if (err)
		return err;
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW, &old_path);
	if (err)
		return err;

	down_write(&namespace_sem);
	err = -EINVAL;
	if (IS_MNT_UNBINDABLE(old_path.mnt))
		goto out;

	if (!check_mnt(path->mnt) || !check_mnt(old_path.mnt))
		goto out;

	err = -ENOMEM;
	if (recurse) {
		mnt = copy_tree_api(old_path.mnt, old_path.dentry, 0);
	}else {
		mnt = clone_mnt_api(old_path.mnt, old_path.dentry, 0);
	}
	if (!mnt)
		goto out;
	err = graft_tree(mnt, path);
	if (err) {
		LIST_HEAD(umount_list);
		spin_lock(&vfsmount_lock);
		umount_tree_api(mnt, 0, &umount_list);
		spin_unlock(&vfsmount_lock);
		release_mounts_api(&umount_list);
	}

out:
	up_write(&namespace_sem);
	path_put(&old_path);
	return err;
}
static int mnt_make_readonly(struct vfsmount *mnt)
{
	int ret = 0;

	spin_lock(&vfsmount_lock);
	mnt->mnt_flags |= MNT_WRITE_HOLD;
	/*
	 * After storing MNT_WRITE_HOLD, we'll read the counters. This store
	 * should be visible before we do.
	 */
	smp_mb();

	/*
	 * With writers on hold, if this value is zero, then there are
	 * definitely no active writers (although held writers may subsequently
	 * increment the count, they'll have to wait, and decrement it after
	 * seeing MNT_READONLY).
	 *
	 * It is OK to have counter incremented on one CPU and decremented on
	 * another: the sum will add up correctly. The danger would be when we
	 * sum up each counter, if we read a counter before it is incremented,
	 * but then read another CPU's count which it has been subsequently
	 * decremented from -- we would see more decrements than we should.
	 * MNT_WRITE_HOLD protects against this scenario, because
	 * mnt_want_write first increments count, then smp_mb, then spins on
	 * MNT_WRITE_HOLD, so it can't be decremented by another CPU while
	 * we're counting up here.
	 */
	if (count_mnt_writers(mnt) > 0)
		ret = -EBUSY;
	else
		mnt->mnt_flags |= MNT_READONLY;
	/*
	 * MNT_READONLY must become visible before ~MNT_WRITE_HOLD, so writers
	 * that become unheld will see MNT_READONLY.
	 */
	smp_wmb();
	mnt->mnt_flags &= ~MNT_WRITE_HOLD;
	spin_unlock(&vfsmount_lock);
	return ret;
}

static void __mnt_unmake_readonly(struct vfsmount *mnt)
{
	spin_lock(&vfsmount_lock);
	mnt->mnt_flags &= ~MNT_READONLY;
	spin_unlock(&vfsmount_lock);
}


static int change_mount_flags(struct vfsmount *mnt, int ms_flags)
{
	int error = 0;
	int readonly_request = 0;

	if (ms_flags & MS_RDONLY)
		readonly_request = 1;
	if (readonly_request == __mnt_is_readonly(mnt))
		return 0;

	if (readonly_request)
		error = mnt_make_readonly(mnt);
	else
		__mnt_unmake_readonly(mnt);
	return error;
}

static int do_remount(struct path *path, int flags, int mnt_flags,
		      void *data)
{
	int err;
	struct super_block *sb = path->mnt->mnt_sb;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (!check_mnt(path->mnt))
		return -EINVAL;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	down_write(&sb->s_umount);
	if (flags & MS_BIND)
		err = change_mount_flags(path->mnt, flags);
	else
		err = do_remount_sb_api(sb, flags, data, 0);
	if (!err) {
		spin_lock(&vfsmount_lock);
		mnt_flags |= path->mnt->mnt_flags & MNT_PROPAGATION_MASK;
		path->mnt->mnt_flags = mnt_flags;
		spin_unlock(&vfsmount_lock);
	}
	up_write(&sb->s_umount);
	if (!err) {
		spin_lock(&vfsmount_lock);
		touch_mnt_namespace_api(path->mnt->mnt_ns);
		spin_unlock(&vfsmount_lock);
	}
	return err;
}

static inline int tree_contains_unbindable(struct vfsmount *mnt)
{
	struct vfsmount *p;
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		if (IS_MNT_UNBINDABLE(p))
			return 1;
	}
	return 0;
}

static int do_move_mount(struct path *path, char *old_name)
{
	struct path old_path, parent_path;
	struct vfsmount *p;
	int err = 0;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW, &old_path);
	if (err)
		return err;

	down_write(&namespace_sem);
	while (d_mountpoint(path->dentry) &&
	       follow_down(path))
		;
	err = -EINVAL;
	if (!check_mnt(path->mnt) || !check_mnt(old_path.mnt))
		goto out;

	err = -ENOENT;
	mutex_lock(&path->dentry->d_inode->i_mutex);
	if (cant_mount(path->dentry))
		goto out1;

	if (d_unlinked(path->dentry))
		goto out1;

	err = -EINVAL;
	if (old_path.dentry != old_path.mnt->mnt_root)
		goto out1;

	if (old_path.mnt == old_path.mnt->mnt_parent)
		goto out1;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(old_path.dentry->d_inode->i_mode))
		goto out1;
	/*
	 * Don't move a mount residing in a shared parent.
	 */
	if (old_path.mnt->mnt_parent &&
	    IS_MNT_SHARED(old_path.mnt->mnt_parent))
		goto out1;
	/*
	 * Don't move a mount tree containing unbindable mounts to a destination
	 * mount which is shared.
	 */
	if (IS_MNT_SHARED(path->mnt) &&
	    tree_contains_unbindable(old_path.mnt))
		goto out1;
	err = -ELOOP;
	for (p = path->mnt; p->mnt_parent != p; p = p->mnt_parent)
		if (p == old_path.mnt)
			goto out1;

	err = attach_recursive_mnt_api(old_path.mnt, path, &parent_path);
	if (err)
		goto out1;

	/* if the mount is moved, it should no longer be expire
	 * automatically */
	list_del_init(&old_path.mnt->mnt_expire);
out1:
	mutex_unlock(&path->dentry->d_inode->i_mutex);
out:
	up_write(&namespace_sem);
	if (!err)
		path_put(&parent_path);
	path_put(&old_path);
	return err;
}
static int do_new_mount(struct path *path, char *type, int flags,
			int mnt_flags, char *name, void *data)
{
	struct vfsmount *mnt;

	if (!type)
		return -EINVAL;

	/* we need capabilities... */
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	lock_kernel();
	mnt = do_kern_mount(type, flags, name, data);
	unlock_kernel();
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	return do_add_mount(mnt, path, mnt_flags, NULL);
}

static long exact_copy_from_user(void *to, const void __user * from,
				 unsigned long n)
{
	char *t = to;
	const char __user *f = from;
	char c;

	if (!access_ok(VERIFY_READ, from, n))
		return n;

	while (n) {
		if (__get_user(c, f)) {
			memset(t, 0, n);
			break;
		}
		*t++ = c;
		f++;
		n--;
	}
	return n;
}

int copy_mount_options(const void __user * data, unsigned long *where)
{
	int i;
	unsigned long page;
	unsigned long size;

	*where = 0;
	if (!data)
		return 0;

	if (!(page = __get_free_page(GFP_KERNEL)))
		return -ENOMEM;

	/* We only care that *some* data at the address the user
	 * gave us is valid.  Just in case, we'll zero
	 * the remainder of the page.
	 */
	/* copy_from_user cannot cross TASK_SIZE ! */
	size = TASK_SIZE - (unsigned long)data;
	if (size > PAGE_SIZE)
		size = PAGE_SIZE;

	i = size - exact_copy_from_user((void *)page, data, size);
	if (!i) {
		free_page(page);
		return -EFAULT;
	}
	if (i != PAGE_SIZE)
		memset((char *)page + i, 0, PAGE_SIZE - i);
	*where = page;
	return 0;
}

int copy_mount_string(const void __user *data, char **where)
{
	char *tmp;

	if (!data) {
		*where = NULL;
		return 0;
	}

	tmp = strndup_user(data, PAGE_SIZE);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	*where = tmp;
	return 0;
}

long do_mount(char *dev_name, char *dir_name, char *type_page,unsigned long flags, void *data_page)
{
	struct path path;
	int retval = 0;
	int mnt_flags = 0;

	printk("=============================================================\n");
	printk("[SAMYGO]mount : %s, %s, %s, %d, %x\n",dev_name,dir_name,type_page,flags,data_page);
	printk("[SAMYGO]PID   : %d  (%s)\n", current->pid, current->comm);
	printk("=============================================================\n");

	/* Discard magic */
	if ((flags & MS_MGC_MSK) == MS_MGC_VAL)
		flags &= ~MS_MGC_MSK;

	/* Basic sanity checks */

	if (!dir_name || !*dir_name || !memchr(dir_name, 0, PAGE_SIZE))
		return -EINVAL;

	if (data_page)
		((char *)data_page)[PAGE_SIZE - 1] = 0;

	/* ... and get the mountpoint */
	retval = kern_path(dir_name, LOOKUP_FOLLOW, &path);
	if (retval)
		return retval;

	retval = security_sb_mount(dev_name, &path,
				   type_page, flags, data_page);
	if (retval)
		goto dput_out;

	/* Default to relatime unless overriden */
	if (!(flags & MS_NOATIME))
		mnt_flags |= MNT_RELATIME;

	/* Separate the per-mountpoint flags */
	if (flags & MS_NOSUID)
		mnt_flags |= MNT_NOSUID;
	if (flags & MS_NODEV)
		mnt_flags |= MNT_NODEV;
	if (flags & MS_NOEXEC)
		mnt_flags |= MNT_NOEXEC;
	if (flags & MS_NOATIME)
		mnt_flags |= MNT_NOATIME;
	if (flags & MS_NODIRATIME)
		mnt_flags |= MNT_NODIRATIME;
	if (flags & MS_STRICTATIME)
		mnt_flags &= ~(MNT_RELATIME | MNT_NOATIME);
	if (flags & MS_RDONLY)
		mnt_flags |= MNT_READONLY;

	flags &= ~(MS_NOSUID | MS_NOEXEC | MS_NODEV | MS_ACTIVE | MS_BORN |
		   MS_NOATIME | MS_NODIRATIME | MS_RELATIME| MS_KERNMOUNT |
		   MS_STRICTATIME);

	if (flags & MS_REMOUNT)
		retval = do_remount(&path, flags & ~MS_REMOUNT, mnt_flags,
				    data_page);
	else if (flags & MS_BIND)
	{
	printk("\n");
		printk("\033[31mNOERROR : BIND option was called with - %s -\033[0m\n", current->comm);
		printk("\033[31m  BIND option can not make any potential problem of system security.\033[0m\n");
		printk("\033[31m  You may use this usefull function on embedded system.\033[0m\n");
		retval = do_loopback(&path, dev_name, flags & MS_REC);
	}
	else if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		retval = do_change_type(&path, flags);
	else if (flags & MS_MOVE)
		retval = do_move_mount(&path, dev_name);
	else
		retval = do_new_mount(&path, type_page, flags, mnt_flags,
				      dev_name, data_page);
dput_out:
	path_put(&path);
	return retval;
}

// asmlinkage long kexec_load(unsigned long entry, unsigned long nr_segments, struct kexec_segment __user *segments, unsigned long flags)
//){
//SYSCALL_DEFINE5(mount, char __user *, dev_name, char __user *, dir_name, char __user *, type, unsigned long, flags, void __user *, data)
asmlinkage long mount( char __user * dev_name, char __user * dir_name, char __user * type, unsigned long flags, void __user * data)
{
	int ret;
	char *kernel_type;
	char *kernel_dir;
	char *kernel_dev;
	unsigned long data_page;

	ret = copy_mount_string(type, &kernel_type);
	if (ret < 0)
		goto out_type;

	kernel_dir = getname(dir_name);
	if (IS_ERR(kernel_dir)) {
		ret = PTR_ERR(kernel_dir);
		goto out_dir;
	}

	ret = copy_mount_string(dev_name, &kernel_dev);
	if (ret < 0)
		goto out_dev;

	ret = copy_mount_options(data, &data_page);
	if (ret < 0)
		goto out_data;

	ret = do_mount(kernel_dev, kernel_dir, kernel_type, flags, (void *) data_page);

	free_page(data_page);
out_data:
	kfree(kernel_dev);
out_dev:
	putname(kernel_dir);
out_dir:
	kfree(kernel_type);
out_type:
	return ret;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Access non-exported symbols");
MODULE_AUTHOR("Dejan Nadarevic");

unsigned long **sys_call_table;
static unsigned long **aquire_sys_call_table(void)
{
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;

	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close) 
			return sct;

		offset += sizeof(void *);
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define HIJACK_SIZE 24
struct sym_hook {
    void *addr;
    unsigned char o_code[HIJACK_SIZE];
    unsigned char n_code[HIJACK_SIZE];
    struct list_head list;
};

LIST_HEAD(hooked_syms);

void cacheflush ( void *begin, unsigned long size )
{
    flush_icache_range((unsigned long)begin, (unsigned long)begin + size);
}


unsigned char hijack_code[] = {	0x0D,0xC0,0xA0,0xE1, // MOV             R12, SP
											0x00,0xD8,0x2D,0xE9,	// STMFD           SP!, {R11,R12,LR,PC}
											0x04,0xB0,0x4C,0xE2,	//	SUB             R11, R12, #4
											0x04,0xF0,0x1F,0xE5,	//	LDR             PC, =addr  ; callback to module function
											0x00,0x00,0x00,0x00, // DCD addr
											0x00,0xA8,0x9D,0xE8};//	LDMFD           SP, {R11,SP,PC}
void hijack_start ( void *target, void *new )
{
    struct sym_hook *sa;
    unsigned char o_code[HIJACK_SIZE], n_code[HIJACK_SIZE];

    if ( (unsigned long)target % 4 == 0 )
    {
        memcpy(n_code, hijack_code, HIJACK_SIZE);
        *(unsigned long *)&n_code[16] = (unsigned long)new;

    }
  
    memcpy(o_code, target, HIJACK_SIZE);

    memcpy(target, n_code, HIJACK_SIZE);
    cacheflush(target, HIJACK_SIZE);

    sa = kmalloc(sizeof(*sa), GFP_KERNEL);
    if ( ! sa )
        return;

    sa->addr = target;
    memcpy(sa->o_code, o_code, HIJACK_SIZE);
    memcpy(sa->n_code, n_code, HIJACK_SIZE);

    list_add(&sa->list, &hooked_syms);
}

void hijack_adress ( void *target, unsigned long new )
{
    struct sym_hook *sa;
    unsigned char o_code[HIJACK_SIZE], n_code[HIJACK_SIZE];

    if ( (unsigned long)target % 4 == 0 )
    {
        *(unsigned long *)&n_code[0] = (unsigned long)new;
    } else {
    	return;	
    }
    

    memcpy(o_code, target, HIJACK_SIZE);

    memcpy(target, n_code, HIJACK_SIZE);
    cacheflush(target, HIJACK_SIZE);

    sa = kmalloc(sizeof(*sa), GFP_KERNEL);
    if ( ! sa )
        return;

    sa->addr = target;
    memcpy(sa->o_code, o_code, HIJACK_SIZE);
    memcpy(sa->n_code, n_code, HIJACK_SIZE);
    list_add(&sa->list, &hooked_syms);
}


void (*Exception_from_authuld)(const unsigned char *msg);
void (*authuld1)(const unsigned char *msg);
void (*execute_authuld)(void);
/*
int test1(void) {
	return 1;	
}

int test(void) {
	asm volatile(
	"ldr pc, __tes_1_func\n\
	__tes_1_func:						\n\
		.word	test1		\n\
		" :
	::
	    "memory" );
}
*/

asmlinkage void n_Exception_from_authuld(const unsigned char *msg) {
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"n_Exception_from_authuld called\n" );
	printk(KERN_ERR"MSG : %s\n", msg );
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");
}

asmlinkage void n_execute_authuld(void) {
	
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"n_execute_authuld called\n" ); // when authuld shall start, it call now this function.
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");	
	return ;
}

void patch_authuld_001(void) {
	unsigned int *pointer = 0xC0025BB0; // 0064
	hijack_adress( pointer , 0xEA000017); 
}

void patch_authuld_002(void) {
	unsigned int *pointer = 0xC01A83E0; // location in kernel

	hijack_adress( pointer , 0xEA000003);   // remove HEX filtering for console
	pointer = 0xC03793F4;  // serial enable
	if ( pointer[0] == 0)
		hijack_adress( pointer , 0x01);  // remove 20102011 enter to enable console imeditly  patch beq to B
	

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








void init_kernel_api(void) {

	init_rwsem(&namespace_sem);
	
	copy_tree_api 					= (struct vfsmount *(*)(struct vfsmount *mnt, struct dentry *dentry,int flag))0xC00D0BB8;
	clone_mnt_api 					= (struct vfsmount *(*)(struct vfsmount *old, struct dentry *root,int flag))0xC00D09FC;
	release_mounts_api 			= (void (*)(struct list_head *head))0xC00CF74C;
	umount_tree_api 				= (void (*)(struct vfsmount *mnt, int propagate, struct list_head *kill)) 0xC00CF1C0;
	attach_recursive_mnt_api	= (int (*)(struct vfsmount *source_mnt,struct path *path, struct path *parent_path))0xC00CFE70;
	invent_group_ids_api		 	=	( int (*)(struct vfsmount *mnt, bool recurse))0xC00CFDF0 ;
	change_mnt_propagation_api	= (void (*)(struct vfsmount *mnt, int type)) 0xC00D5FC0;
	touch_mnt_namespace_api		= (void (*)(struct mnt_namespace *ns)) 0xC00CE944;
	do_remount_sb_api				= (int (*)(struct super_block *sb, int flags, void *data, int force))0xC00B9674;



}

static int __init api_mount_init(void)
{

	init_kernel_api();
	if(!(sys_call_table = aquire_sys_call_table()))
		return -1;
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"move mount from %x to %x\n", (unsigned int)sys_call_table[__NR_mount] ,(unsigned int)mount );
	printk(KERN_ERR"=============================================================\n");

	mount_orig = sys_call_table[__NR_mount];
	sys_call_table[__NR_mount] = mount;
	
	
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"authuld_init\n" );
	printk(KERN_ERR"Exception_from_authuld located at %x replaced by %x\n" , 0xC00255B8, (unsigned int) n_Exception_from_authuld  );
	printk(KERN_ERR"=============================================================\n");
	Exception_from_authuld	=  (void (*)(const unsigned char *msg))0xC00255B8; // 0064
	execute_authuld			= (void (*)(void))0xC0025918; // 0064
//	authuld1 					= (void (*)(const unsigned char *msg))0xC0028B18; // 0084
	hijack_start(execute_authuld, &n_execute_authuld);
//	hijack_start(authuld1, &n_authuld1);
	hijack_start(Exception_from_authuld, &n_Exception_from_authuld);
	patch_authuld_001();
	patch_authuld_002();
	return 0;
}

static void __exit api_mount_exit(void)
{
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"move mount back to %x\n", (unsigned int)mount_orig);
	printk(KERN_ERR"=============================================================\n");
	sys_call_table[__NR_mount] = mount_orig; 
}

module_init(api_mount_init);
module_exit(api_mount_exit);
