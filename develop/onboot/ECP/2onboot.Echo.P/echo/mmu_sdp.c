#define TTB_ADDR	0x6FD00000
static unsigned int* mmu_table_1st=TTB_ADDR;

/* Domain = 0b0001, AP = full */
#define LV1_MEMORY		0xc2e // c:on b:on
#define LV1_STRONG_ORD	0xc22 // c:off b:off
#define LV1_DEVICE		0xc26 // c:off b:on

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
typedef enum { MAP_SECTION, MAP_PAGE } MMU_MAP_TYPE;

struct mapping_info {
	unsigned int	va;
	unsigned int	pa;
	unsigned int	len;
	MMU_MAP_TYPE	type;
	unsigned int	prot;
};

struct mapping_info mapinfo[] = {
	/* VA         PA          SIZE(1MB min)   TYPE          PROT */
	{ 0x00000000, 0x00000000, 0x100000, MAP_SECTION, LV1_STRONG_ORD},
	{ 0x04000000, 0x04000000, 0x100000, MAP_SECTION, LV1_STRONG_ORD},
	{ 0x30000000, 0x30000000, 0x1000000, MAP_SECTION, LV1_STRONG_ORD},
	{ 0x60000000, 0x60000000, 0x500000, MAP_SECTION, LV1_DEVICE}, /* kernel loading region */
	{ 0x67000000, 0x67000000, 0x200000, MAP_SECTION, LV1_MEMORY}, /* onboot loading region */
	{ 0x67400000, 0x67400000, 0x100000, MAP_SECTION, LV1_DEVICE}, /* cmac dma region */
	{ 0x67C00000, 0x67C00000, 0x100000, MAP_SECTION, LV1_DEVICE}, /* uboot loading region */

	/* onenand area */
	{ 0x10000000, 0x00000000, 0x100000, MAP_SECTION, LV1_MEMORY},
	{ 0x10400000, 0x00400000, 0x100000, MAP_SECTION, LV1_MEMORY},
};

static void mmu_gen_table(struct mapping_info *info)
{
	unsigned int va = info->va;
	unsigned int pa = info->pa;
	unsigned int end = va + info->len;
	MMU_MAP_TYPE type = info->type;
	unsigned int prot = info->prot;

	if (type == MAP_SECTION) {
		for (; va<end; va+=0x100000, pa+=0x100000)
			mmu_table_1st[va>>20] = pa | prot;
	} else {
		 /* not Implemented */
	}
}

void mmu_init(void)
{
	int i;
	volatile unsigned int reg;

	/* Setup MMU Table */
	for (i=0; i<ARRAY_SIZE(mapinfo); i++) {
		mmu_gen_table(&mapinfo[i]);
	}

	/* DACR : all permision to all domains */
	reg = 0xffffffff;
	asm volatile("mcr p15, 0, %0, c3, c0, 0" ::"r" (reg));

	/* TTB0 */
	reg = (unsigned int)mmu_table_1st;
	reg |= 0x18;	/* outer wb/wa, inner ns/nc */
	asm volatile("mcr p15, 0, %0, c2, c0, 0" ::"r" (reg));

	/* Enable MMU */
	asm volatile("mrc p15, 0, %0, c1, c0, 0" "\n\t"
				 "orr %0, %0, #0x00000005" "\n\t"     /* set bits (-C-M)*/
				 "mcr p15, 0, %0, c1, c0, 0" "\n\t"
	    		 "mrc p15, 0, %0, c0, c0, 0" "\n\t"
    			 "mov %0, %0" "\n\t"
    			 "mov %0, %0":"=r" (reg));
}

void dcache_flush_all(void);
inline void mmu_disable(void)
{
	volatile unsigned int reg;	
	asm volatile("mrc p15, 0, %0, c1, c0, 0" "\n\t"
				 "bic %0, %0, #0x00000005" "\n\t"     /* clear bits (-C-M)*/
				 "mcr p15, 0, %0, c1, c0, 0" "\n\t"
	    		 "mrc p15, 0, %0, c0, c0, 0" "\n\t"
    			 "mov %0, %0" "\n\t"
    			 "mov %0, %0":"=r" (reg));
	dcache_flush_all();
}

inline void dmb(void)
{
	asm volatile ("dmb	");
}

inline void dcache_flush(int bL2)
{
	unsigned int tmp;
	unsigned int start,end,wayOffset;
	
	if (!bL2)  	/* L1 Cache */
	{
		start = 0x0;
		end = 0x1fc0;
		wayOffset=(1<<30);
	}
	else		/* L2 Cache */
	{
		start = 0x2; 
		end = 0xffc2; 
		wayOffset=(1<<29);
	}

	/* invalidate d-cache by Set/Way */
	asm volatile (
		"mov	%0, %1\n\t"
		"1:mcr	p15, 0, %0, c7, c14, 2\n\t"
		"add	%0, %0, #64\n\t"
		"cmp	%0, %2\n\t"
		"blt	1b\n\t"
		"add	%0, %1, %3\n\t"
		"adds	%2, %2, %3\n\t"
		"bcc	1b \n\t": "=r"(tmp) : "r"(start),"r"(end),"r"(wayOffset) 
	);
}

/* line size = 64 bytes */
inline void dcache_invalidate_mva(unsigned int addr, unsigned int len)
{
	unsigned int tmp;
	unsigned int mva_start, mva_end;

	mva_start = addr & (~63);
	mva_end = ((addr + len) + 63) & (~63);

	/* invalidate d-cache by VA */
	asm volatile (
		"mov	%0, %1\n\t"
		"1:mcr	p15, 0, %0, c7, c6, 1\n\t"
		"add	%0, %0, #64\n\t"
		"cmp	%0, %2\n\t"
		"blt	1b" : "=r"(tmp) : "r"(mva_start), "r"(mva_end)
	);
}

#if 0
inline void dcache_clean_mva(unsigned int addr, unsigned int len)
{
	unsigned int tmp;
	unsigned int mva_start, mva_end;

	mva_start = addr & (~63);
	mva_end = ((addr + len) + 63) & (~63);

	/* clean d-cache by VA */
	asm volatile (
		"mov	%0, %1\n\t"
		"1:mcr	p15, 0, %0, c7, c10, 1\n\t"
		"add	%0, %0, #64\n\t"
		"cmp	%0, %2\n\t"
		"blt	1b" : "=r"(tmp) : "r"(mva_start), "r"(mva_end)
	);
}
#endif  

/* clean & invalidate all */
void dcache_flush_all(void)
{
	dcache_flush(0); // L1 flush
	dcache_flush(1); // L2 flush
}

