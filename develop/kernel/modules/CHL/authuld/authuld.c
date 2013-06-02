#include <linux/types.h>
#include <linux/major.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/devpts_fs.h>
#include <linux/file.h>

#include <linux/console.h>
#include <linux/timer.h>
#include <linux/ctype.h>
#include <linux/kd.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/serial.h>

#include <linux/uaccess.h>
#include <asm/system.h>

#include <linux/kbd_kern.h>
#include <linux/vt_kern.h>
#include <linux/selection.h>

#include <linux/kmod.h>

#include <linux/slab.h>
#include <asm/cacheflush.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Access non-exported symbols");
MODULE_AUTHOR("Dejan Nadarevic");

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
    else // Thumb
    {
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
	printk(KERN_ERR"n_execute_authuld called\n" );
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");	
	return ;
}

asmlinkage void n_authuld1(const unsigned char *msg) {
	
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"n_authuld1 called\n" );
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"=============================================================\n");	
	return ;
}

void patch_authuld_001(void) {
	unsigned int *pointer = 0xC001F500;
	printk(KERN_ERR"patch_authuld_001 : %x\n",(unsigned int) pointer[0] );
	hijack_adress( pointer , 0xEA000026); 
}

void patch_authuld_002(void) {
//	unsigned int *pointer = 0xC01CB454;
//	printk(KERN_ERR"patch_authuld_002 : %x\n",(unsigned int) pointer[0] );
//	hijack_adress( pointer , 0xEAFFFEC5); 
}

void console_unlock(void) {

	unsigned int *pointer = 0xC01665B4;
	printk(KERN_ERR"patchconsole : %x\n",(unsigned int) pointer[0] );
	hijack_adress( pointer , 0xEA000002); 
	
}


unsigned int *pointer = 0xC00288E4;
static int __init authuld_init(void)
{
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"authuld_init\n" );
	printk(KERN_ERR"Exception_from_authuld located at %x replaced by %x\n" , 0xC001F068, (unsigned int) n_Exception_from_authuld  );
	printk(KERN_ERR"=============================================================\n");

	Exception_from_authuld	=  (void (*)(const unsigned char *msg))0xC001F068;
	execute_authuld			= (void (*)(void))0xC001F0CC;
	//authuld1 					= (void (*)(const unsigned char *msg))0xC0028B38;
	hijack_start(execute_authuld, &n_execute_authuld);
	//hijack_start(authuld1, &n_authuld1);
	hijack_start(Exception_from_authuld, &n_Exception_from_authuld);
	patch_authuld_001();
	console_unlock();
	
	//patch_authuld_002();
	return 0;
}

static void __exit authuld_exit(void)
{
	
	printk(KERN_ERR"=============================================================\n");
	printk(KERN_ERR"authuld_exit\n" );
	printk(KERN_ERR"=============================================================\n");

	
}

module_init(authuld_init);
module_exit(authuld_exit);
