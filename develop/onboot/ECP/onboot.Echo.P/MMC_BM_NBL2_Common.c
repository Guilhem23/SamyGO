/**
 *   @mainpage   Flex Sector Remapper
 *
 *   @section Intro
 *       Flash Translation Layer for Flex-OneNAND and OneNAND
 *    
 *    @section  Copyright
 *            COPYRIGHT. 2003-2007 SAMSUNG ELECTRONICS CO., LTD.               
 *                            ALL RIGHTS RESERVED                              
 *                                                                             
 *     Permission is hereby granted to licensees of Samsung Electronics        
 *     Co., Ltd. products to use or abstract this computer program for the     
 *     sole purpose of implementing a product based on Samsung                 
 *     Electronics Co., Ltd. products. No other rights to reproduce, use,      
 *     or disseminate this computer program, whether in part or in whole,      
 *     are granted.                                                            
 *                                                                             
 *     Samsung Electronics Co., Ltd. makes no representation or warranties     
 *     with respect to the performance of this computer program, and           
 *     specifically disclaims any responsibility for any damages,              
 *     special or consequential, connected with the use of this program.       
 *
 *     @section Description
 *
 */
 
/**
 * @file      FSR_BM_BL2_Common.c
 * @brief     This is second boot loader of OneNAND
 * @author    JeongWook Moon
 * @author    SongHo Yoon
 * @date      19-MAR-2007
 * @remark
 * REVISION HISTORY
 * @n  19-MAR-2007 [JeongWook Moon] : First writing
 * @n  20-AUG-2007 [SongHo Yoon]  : reorganization
 *
 */
 
/*****************************************************************************/
/* Header file declararation                                                 */
/*****************************************************************************/
#include <version.h>
#include <sp_setup.h>
#include <types.h>
#include <sdp_devmmc.h>
#include <config.h>

#if defined(USE_PTM)
#define I2C_PORT_A		1
#define ECHOA_CHIP_ID	0xAE
#endif

#if defined(BOARD_OD)

#else //#if defined(BOARD_OD)
	#if defined(SILENT_KERNEL)

		#if defined(CONFIG_SMP)

		char* kernel_cmdline_rfs0="root=/dev/mmcblk0p3 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" quiet \0";
		char* kernel_cmdline_rfs1="root=/dev/mmcblk0p6 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" quiet \0";
		char* kernel_cmdline_ext0="root=/dev/mmcblk0p13 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_ext1="root=/dev/mmcblk0p14 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_sda1="root=/dev/sda1 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_nfs="root=/dev/nfs console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		#else
		#endif
	#else // else SILENT_KERNEL
		#if defined(CONFIG_SMP)
		char* kernel_cmdline_rfs0="root=/dev/mmcblk0p3 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_rfs1="root=/dev/mmcblk0p6 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_ext0="root=/dev/mmcblk0p13 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_ext1="root=/dev/mmcblk0p14 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_sda1="root=/dev/sda1 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		char* kernel_cmdline_nfs="root=/dev/nfs console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot="SAMSUNG_BOOTLOADER_VERSION" \0";
		#else
	
		#endif
	#endif
#endif

#if defined(SILENT_KERNEL)
#define printf 
#else
#define printf PRINTF
#endif

#if defined(FAST_LOGO)

#define I2C_PORT_NUM    4
#define PARMA_I2C_PORT_NUM  4 // PARMA
#define NEC_CHIP_ID     0xCE
#define PARMA_CHIP_ID   0xC0

#define LOWBYTE(w)              ((w) & 0x00ff)
#define HIGHBYTE(w)             (((w) >> 8) & 0x00ff)

// LD Bypass mode
// 1. Initial table (2010-11-12)
static unsigned short pu32LD_Inital_FastBoot[] = {
    0x029F, 0x0010,
    0x02A2, 0x0000,
    0x004B, 0x0010,
    0x028F, 0x0000,
    0x0269, 0x0000,
    0x0282, 0x0000,
    0x02BF, 0x0000,
    0x0286, 0x0000,
    0x03A1, 0x0055,
    0x03A2, 0x0005,
    0x0395, 0x0013,
    0x039E, 0x000D,
    0x0089, 0x1FFF,
    0x006D, 0x0001,
    0x0354, 0x0000,
    0x0356, 0x0001
};

// Parma Bypass mode
// released 2011.11.08
static unsigned short pu32ParmaInitialize_FullBypass_PDP[] = {
	0x0F56 , 0x03CA , //DH_END
	0x0F6C , 0x0001 , //POST_MODE	 
	0x0015 , 0x0001 , //CLK_O_SEL	 	
	0x0023 , 0x0000 , //PRE_POST_BYP_EN	
	0x0F8E , 0x0000 , //LVDS_TX_PWRDN
	0x0F6A , 0x0000, //LVDS_TX_FMT 
	0x0024 , 0x0005 , //FULL_BYP_DATA_SEL1
	0x0025 , 0x0004 , //FULL_BYP_DATA_SEL2
	0x0026 , 0x0005 , //FULL_BYP_DATA_SEL3
	0x0027 , 0x0004 , //FULL_BYP_DATA_SEL4		
	0x023B , 0x0001 , //LVDS_RGB_SEL		    
};

int fast_logo(int inverter_on_time);
#endif //defined(FAST_LOGO) 

int high_capacity;

static void _SelectBootMode        (unsigned int  PartitionID);
extern void Uart_SendString (char *s);
extern void sdp_peri_init(void);
extern void sdp_hci_tx_init(void);

struct mmc *mmc = NULL;

#if defined(CONFIG_SDP_MEMTAG)

#define DDRA_MAX_SIZE	(512 << 20)
#define DDRB_MAX_SIZE	(512 << 20)
#define DDRC_MAX_SIZE	(512 << 20)

struct sdp_mem_param_t {
	char magic[16];
	u32  sys_mem0_size;
	u32  mach_mem0_size;
	u32  sys_mem1_size;
	u32  mach_mem1_size;
	u32  sys_mem2_size;
	u32  mach_mem2_size;
};

void setup_sdp_memtag()
{
	volatile struct sdp_mem_param_t *param = (struct sdp_mem_param_t *)MEM_TAG_ADDR;;
	const char sdp_magic[] = "SamsungCCEP_memi";

	memset (param, 0, sizeof(*param));

#if defined(BOARD_OD)
    param->sys_mem0_size = 772;
    param->mach_mem0_size = 1024;
    param->sys_mem1_size = 144;
    param->mach_mem1_size = 256;
    param->sys_mem2_size = 180;
    param->mach_mem2_size = 256;
#else // Pre1, Pre2, DV, PV
#if defined(MEMORY_MAP_1024MB) // PV
    param->sys_mem0_size = 260;
    param->mach_mem0_size = 512;
    param->sys_mem1_size = 144;
    param->mach_mem1_size = 256;
    param->sys_mem2_size = 180;
    param->mach_mem2_size = 256;
#elif defined(MEMORY_MAP_1536MB) // Pre1, Pre2, DV
    param->sys_mem0_size = 656;
    param->mach_mem0_size = 1024;
    param->sys_mem1_size = 0;
    param->mach_mem1_size = 256;
    param->sys_mem2_size = 0;
    param->mach_mem2_size = 256;
#endif
#endif

	/* Check */
	if (param->sys_mem0_size > param->mach_mem0_size ||
		param->sys_mem1_size > param->mach_mem1_size ||
		param->sys_mem2_size > param->mach_mem2_size ||
		param->mach_mem0_size > DDRA_MAX_SIZE ||
		param->mach_mem1_size > DDRB_MAX_SIZE ||
		param->mach_mem2_size > DDRC_MAX_SIZE ||
		!(param->sys_mem0_size || param->sys_mem1_size || param->sys_mem2_size) ||
		!(param->mach_mem0_size || param->mach_mem1_size || param->mach_mem2_size)) {
		PRINTF ("WARNING: kernel memory argumets are bad. check environment arguments.");
	} else {
		memcpy (param->magic, sdp_magic, sizeof(param->magic));
	}
}


#endif


int atoi(char *str)
{
	int i;
	int num = 0;
	for(i=0; str[i] != NULL; i++)
	{
		if('0' <= str[i] && str[i] <= '9')
		{
			num *= 10;
			num += str[i] - 48;
		}
	}
	return num;
}

/*****************************************************************************/
/* Code Implementation                                                       */
/*****************************************************************************/

static inline void markKernelCMD(int flag)
{
	struct tag *kernel_tag = (struct tag*)CMDLINE_TAG_ADDR;

	char *kernel_cmdline;

	if(flag == PARTITION_ID_KERNEL0)
	{
		kernel_cmdline = kernel_cmdline_rfs0;
	}
	else
	{
		kernel_cmdline = kernel_cmdline_rfs1;
	}
	memset(kernel_tag, 0x00, TAG_SIZE);
 
	kernel_tag = setup_start_tag(kernel_tag);
	kernel_tag = setup_commandline_tag(kernel_tag, kernel_cmdline);
	kernel_tag = setup_end_tag(kernel_tag);
	/* from Genoa-S, kernel may need memory info */
	setup_sdp_memtag();

}

static inline void markKernel_ext3(int flag)
{
	struct tag *kernel_tag = (struct tag*)CMDLINE_TAG_ADDR;

	char *kernel_cmdline;

	if(flag == PARTITION_ID_KERNEL0)
	{
		kernel_cmdline = kernel_cmdline_ext0;
	}
	else
	{
		kernel_cmdline = kernel_cmdline_ext1;
	}
	memset(kernel_tag, 0x00, TAG_SIZE);
 
	kernel_tag = setup_start_tag(kernel_tag);
	kernel_tag = setup_commandline_tag(kernel_tag, kernel_cmdline);
	kernel_tag = setup_end_tag(kernel_tag);
	/* from Genoa-S, kernel may need memory info */
	setup_sdp_memtag();
}

static inline void markKernel_nfs(int flag)
{
	struct tag *kernel_tag = (struct tag*)CMDLINE_TAG_ADDR;

	char *kernel_cmdline;

	if(flag == PARTITION_ID_KERNEL0)
	{
		kernel_cmdline = kernel_cmdline_nfs;
	}
	else
	{
		kernel_cmdline = kernel_cmdline_nfs;
	}
	memset(kernel_tag, 0x00, TAG_SIZE);
 
	kernel_tag = setup_start_tag(kernel_tag);
	kernel_tag = setup_commandline_tag(kernel_tag, kernel_cmdline);
	kernel_tag = setup_end_tag(kernel_tag);
	/* from Genoa-S, kernel may need memory info */
	setup_sdp_memtag();
}

static inline void markKernel_sda1(int flag)
{
	struct tag *kernel_tag = (struct tag*)CMDLINE_TAG_ADDR;

	char *kernel_cmdline;

	if(flag == PARTITION_ID_KERNEL0)
	{
		kernel_cmdline = kernel_cmdline_sda1;
	}
	else
	{
		kernel_cmdline = kernel_cmdline_sda1;
	}
	memset(kernel_tag, 0x00, TAG_SIZE);
 
	kernel_tag = setup_start_tag(kernel_tag);
	kernel_tag = setup_commandline_tag(kernel_tag, kernel_cmdline);
	kernel_tag = setup_end_tag(kernel_tag);
	/* from Genoa-S, kernel may need memory info */
	setup_sdp_memtag();
}

static unsigned int readl_reg(unsigned int off)
{
	return *(volatile unsigned int*)(off);
}

static void writel_reg(unsigned int val, unsigned int off)
{
	*((volatile unsigned int*)(off)) = val;
}

static void auth_kernel(unsigned int kaddr, unsigned int len)
{
	unsigned int *cmac;
	unsigned int stc = 0;
	PRINTF("NKA\r\n");
	return;
	if (readl_reg(0x303f0038) & 1) 
	{
#if !defined(SILENT_KERNEL)
		printf("[H/W]Secure boot disabled. skip authentification stage.\r\n");
#else
		PRINTF("SPD\r\n");
#endif
		return;
	} 
	else 
	{
		printf("Secure boot for kernel: ");
	}

	if (len > 0x1000000) 
	{
#if !defined(SILENT_KERNEL)
		printf("bad image. skip authentification.\n");
#else
		PRINTF("BI\r\n");
		while(1);
#endif
		return;
	}

	stc = *(volatile unsigned int*)0x30110080;
	cmac = (unsigned int *)(kaddr + len - 16);
	writel_reg (kaddr, 0x303f0420);		//start addr
	writel_reg (len - 16, 0x303f0414);	//size
	writel_reg (len - 16, 0x303f0464);	//size
	writel_reg (0xc3, 0x303f0430);		//control
	writel_reg (cmac[0], 0x303f0454);
	writel_reg (cmac[1], 0x303f0458);
	writel_reg (cmac[2], 0x303f045c);
	writel_reg (cmac[3], 0x303f0460);
	writel_reg (0x23, 0x303f0404);			//start auth
	
	/* XXX: clean dcache lines before here if cacheable */
//	writel_reg (0x11, 0x14);
	//wait dma transfer
	while (readl_reg(0x303f0400) != 0x0) {}
	//check auth
	if(readl_reg(0x303f0494) != 0x1)
	{
		PRINTF("AF\r\n");
#if defined(SILENT_KERNEL)
        while(1);
#endif	
	}
	else
	{
		stc = (*(volatile unsigned int*)0x30110080) - stc;
#if !defined(SILENT_KERNEL)
		printf("success: %d-%d.\r\n", len, stc);
#endif
	}
}

unsigned long get_uImage_size(unsigned long partition, unsigned int pSrc)
{
	image_header_t *hdr;
	unsigned int ret;
	int err;

    if((*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR)){
	    err=sdp_mmc_RW(READ_MMC_MODE, partition, (void *)pSrc, BLK_SIZE_MMC*2);  /* Read 1st LBA */
    }else{
	    err=sdp_mmc_RW(READ_MMC_MODE, partition*BLK_SIZE_MMC, (void *)pSrc, BLK_SIZE_MMC*2);  /* Read 1st LBA */
    }
	if(err != SDP_MMC_SUCCESS){
		return err;
	}
    
	hdr = (image_header_t*)pSrc;

	ret = ___swab32(hdr->ih_size);
	return ret;
}

/**
 * @brief          This function calls copy routine and launch next image(OS or TBL)
 * @n
 *
 * @param[in]      nVol     : Volume Number
 *
 * @return         Start Address of OS Image
 * @n              After Image Launch, Never return
 *
 * @since          since v1.0.0
 *
 */

UINT32
_ShadowOS(UINT32 nVol, UINT32 PartitionID)
{
	UINT32          nOsShadowAddr;

	unsigned int kaddr, klen;

	UINT32           nRet;
	UINT32          n1stVpn;
	UINT32          nNumOfPgs;
	UINT32          nPgsPerUnit;
	int i,err;

	unsigned int uImage_size = 0;
	unsigned long rest_uImage_block = 0;
	unsigned long partition = 0;
	unsigned int pSrc;
    
	unsigned int os_kernel_addr = 0;
	unsigned int os_rootfs_addr = 0;

	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");
    
	os_kernel_addr = OS_KERNEL_ADDR;

	if ((PartitionID == PARTITION_ID_KERNEL0) || (PartitionID == PARTITION_ID_KERNEL1))
	{
        if (PartitionID == PARTITION_ID_KERNEL0){
    		printf("loading kernel first part.[mmc : %d]",(*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR));
    		partition = KERNEL_1;		
        }
        else if(PartitionID == PARTITION_ID_KERNEL1){ 
    		printf("loading kernel second part.[mmc : %d]",(*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR));
    		partition = KERNEL_2;		
        }
#ifdef USE_UIMAGE_HEADER
    	pSrc = os_kernel_addr-64 ; // For uImage , 64bit is uImage header size

    	/* File(uImage) size = ih_size(Image size) + 64(header size) */
        uImage_size  = get_uImage_size(partition, pSrc) + 64;
        printf("[%d Byte]\n",uImage_size);

    	rest_uImage_block = (uImage_size / BLK_SIZE_MMC);
        if((*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR)){
        	err=sdp_mmc_RW(READ_MMC_MODE, (partition+2), ((void*)pSrc + BLK_SIZE_MMC*2), rest_uImage_block*BLK_SIZE_MMC);
        }else{
        	err=sdp_mmc_RW(READ_MMC_MODE, (partition+2)*BLK_SIZE_MMC, ((void*)pSrc + BLK_SIZE_MMC*2), rest_uImage_block*BLK_SIZE_MMC);
        }
    	if(err != SDP_MMC_SUCCESS){
    		return err;
    	}	
		//printf("finished loading kernel.\n");
        
    	kaddr = pSrc;
    	klen = ((uImage_size + 16) + 15) & (~15);

    	auth_kernel(kaddr, klen);
#else        
        if((*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR)){
        	err=sdp_mmc_RW(READ_MMC_MODE, partition, os_kernel_addr-64, KERNEL_1_SIZE*BLK_SIZE_MMC);
        }else{
        	err=sdp_mmc_RW(READ_MMC_MODE, partition*BLK_SIZE_MMC, os_kernel_addr-64, KERNEL_1_SIZE*BLK_SIZE_MMC);
        }
    	if(err != SDP_MMC_SUCCESS){
    		return err;
    	}	
        
#endif
        
    	nOsShadowAddr = os_kernel_addr;     
        
	}
	else if (PartitionID == PARTITION_ID_U_BOOT)
	{
		PRINTF("loading u-boot (to 0x67c00000)[mmc : %d]\n",(*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR));
        
		nOsShadowAddr = OS_UBOOT_ADDR;
        if((*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR)){        
        	err=sdp_mmc_RW(READ_MMC_MODE, U_BOOT, OS_UBOOT_ADDR, U_BOOT_SIZE*BLK_SIZE_MMC);
        }else{
        	err=sdp_mmc_RW(READ_MMC_MODE, U_BOOT*BLK_SIZE_MMC, OS_UBOOT_ADDR, U_BOOT_SIZE*BLK_SIZE_MMC);
        }
    	if(err != SDP_MMC_SUCCESS){
    		PRINTF("mmc read error\n");         
    		return err;
    	}	
	}
        
	return (nOsShadowAddr);
}
#if defined(FAST_LOGO)
int fast_logo(int inverter_on_time)
{
	unsigned int addr;	
	unsigned short data;	
	unsigned char buffer[2] = {0, 0};

	int i = 0;
	int ret = 0;		

	unsigned int led_board = 0;
	
	//FSR_OAM_DbgMsg("S1\r\n");

	// o_HSYNC_LC
	// 0: PDP, 1: LED
	// requested by hs23.lee
	*(volatile unsigned int*)0x30090db4 |= (0x2 << 16); //o_HSYNC_LC GPIO 15.4 In
	led_board = (*(volatile unsigned int*)0x30090dbc & 0x10) >> 4; // read 15.4
	printf("Echo-P Board is [%d] = 0:PDP, 1:LED\r\n", led_board);

	if(led_board) /* LED Board  (1:LED,  0:PDP) */
	{
		/* NEC control */
		i2c_init(I2C_PORT_NUM, NEC_CHIP_ID, 200);
		//printf("IIC : Send NEC 0006 0000.... \r\n");

		/* NEC Control by 0x0006. 0x0000 to auto run */
		buffer[0] = 0x00;
		buffer[1] = 0x00;			
		ret = i2c_write(I2C_PORT_NUM, NEC_CHIP_ID, 0x0006, buffer , 2);
		
		/* PVCC On and Set Timer of Inverter On */
		micom_cmd(0x51, inverter_on_time, 0); 
	}
	else /* PDP Board  (1:LED,  0:PDP) */
	{
		/* Parma Reset */
		micom_cmd(0xcc,1,0); // Same as LD Reset
	
		/* Sleep 10ms */
		//__udelay(10 * 1000);
	
		/* Parma Set Bypass Mode */
		i2c_init(PARMA_I2C_PORT_NUM , PARMA_CHIP_ID, 400);
	
		for(i = 0; i < sizeof(pu32ParmaInitialize_FullBypass_PDP)/2 ; i += 2 )
		{
			buffer[0] = HIGHBYTE(pu32ParmaInitialize_FullBypass_PDP[i+1]);
			buffer[1] = LOWBYTE(pu32ParmaInitialize_FullBypass_PDP[i+1]);
	
			/* port, chip, addr, buffer, len */
			ret = i2c_write(PARMA_I2C_PORT_NUM, PARMA_CHIP_ID, pu32ParmaInitialize_FullBypass_PDP[i], buffer, 2);
			if(ret == -1)
			{
				printf("IIC : Parma Set Bypass mode [%d] error.... \r\n", pu32ParmaInitialize_FullBypass_PDP[i]);
				break;
			}
	
			/* FIXME !!!! */
			//udelay(3 * 1000);			
		}

#if 0 // for Debug.. Read Parma Regs.
		__udelay(10000);
	
		for(i = 0; i < sizeof(pu32ParmaInitialize_FullBypass_PDP)/2 ; i += 2 )
		{
			buffer[0] = 0;
			buffer[1] = 0;
	
			/* port, chip, addr, buffer, len */
			ret = i2c_read(PARMA_I2C_PORT_NUM, PARMA_CHIP_ID, pu32ParmaInitialize_FullBypass_PDP[i], buffer, 2);
			if(ret == -1)
			{
				printf("IIC : Read Parma Regs error.... \r\n");
			}
			printf("Parma Read [%d] = [%d][%d] \r\n", pu32ParmaInitialize_FullBypass_PDP[i], buffer[0], buffer[1]);
	
			__udelay(10000);
		}
#endif
	
		/* Set Time of Inverter On */
		// Does not need at PDP, only for LCD
		//micom_cmd(0x51, inverter_on_time, 0);

	}

	//printf("S2\r\n");	
	
	return ret;						
}
#endif //defined(FAST_LOGO)

void (*_RunImage)(int dummy, int mach_code);

#if defined(USE_MMU)
extern void mmu_init();
extern void mmu_disable();
#endif

static void
_SelectBootMode(UINT32  PartitionID)
{
	UINT8  *pJumpAddr = NULL;	

	pJumpAddr = (UINT8 *)_ShadowOS(0, PartitionID);
	
	if (pJumpAddr != NULL)
	{
		_RunImage = (void(*)(int, int))pJumpAddr;
		printf("Jump Kernel...\n");
		_RunImage(0, 0);
	}
	else
	{
		while (1);	        /* ERROR */
	}
}
static inline void markVersion(UINT32 flag)
{
#if !defined(SILENT_KERNEL)
	PRINTF("\n=====================================\n");
#if defined(BOARD_OD)
	PRINTF("onboot Echo-P OD 1536 (" __DATE__ " - " __TIME__ ")\n");
#elif defined(MEMORY_MAP_1536MB) // insert Memory size
	PRINTF("onboot Echo-P 1536 (" __DATE__ " - " __TIME__ ")\n");
#else
	PRINTF("onboot Echo-P 1024 (" __DATE__ " - " __TIME__ ")\n");
#endif
	PRINTF("version     : " SAMSUNG_BOOTLOADER_VERSION "\n");
	PRINTF("ETC         : " SAMSUNG_BOOTLOADER_ETC "\n");
	PRINTF("=====================================\n");
#else
#if defined(BOARD_OD)
	PRINTF("\r\n%d 6 9P "SAMSUNG_BOOTLOADER_VERSION" (" __DATE__ "-" __TIME__ ")\r\n", flag?2:1);
#elif defined(MEMORY_MAP_1536MB) // insert Memory size, 6 means 1536MB, 4 means 1024MB
	PRINTF("\r\n%d 6 P "SAMSUNG_BOOTLOADER_VERSION" (" __DATE__ "-" __TIME__ ")\r\n", flag?2:1);
#else
	PRINTF("\r\n%d 4 P SamyGO "SAMSUNG_BOOTLOADER_VERSION" (" __DATE__ "-" __TIME__ ")\r\n", flag?2:1);
	PRINTF("Hit - for BootMenue\n");
			
#endif
#endif
}
static int getPartFlag(void)
{
    /* UPDATE_FLAG P0.7 : SET CONTROL MODE */
	*(volatile unsigned int*)0x30090d00 |= (0x2 << 28);     /* SET P0.7 TO INPUT MODE*/
    
    /* UPDATE_FLAG P0.7 : READ */
    return (*(volatile unsigned int*)0x30090d08)>>7 & 0x1;
}
static void ResetUsbHub()
{
 	/* LAN_NRESET GPIO P0.0*/
	*(volatile unsigned int*)0x30090d00 |= (0x3);
	(*(volatile unsigned int*)0x30090d04) |= (0x1);
}

void FastLogo(void) {
	
	#if defined(FAST_LOGO)
	#if defined(SILENT_KERNEL)
		fast_logo(100); /* Inverter on time : 1000ms requested by wayward.lee */
	#else
		fast_logo(150); /* Inverter on time : 1500ms requested by hs23.lee */
	#endif
	#endif // defined(FAST_LOGO)
}

/**
 * @brief          This function is Main for NBL2
 * @n
 * @param[in]      None
 * @return         None
 * @since          since v1.0.0
 */
void Main(UINT32  argc, void   *argv)
{
	unsigned char buffer[2] = {0, 0};
	int ret = 0;
	char rc = 0;
	int part_flag = 0;
	sdp_peri_init();    
//    sdp_hci_tx_init();
	timer_init();
	serial_init();

	part_flag = getPartFlag();
    markVersion(part_flag);

#if defined(USE_PTM)
	i2c_init(I2C_PORT_A, ECHOA_CHIP_ID, 400);
	buffer[0] = 0x1E;
	buffer[1] = 0x00;
	ret = i2c_write(I2C_PORT_A, ECHOA_CHIP_ID, 0x0804, buffer , 2);
#endif

    ResetUsbHub();

//#if !defined(SILENT_KERNEL) 
	rc = serial_getc_check(200000);
	
	if(rc == 126) //'~'
	{
		_SelectBootMode(PARTITION_ID_U_BOOT);		
	} else if (rc == 35) {
		part_flag = 1;	
		PRINTF("PARTFLAG TO 2 changed\n");
	} else if ( rc == 45 ) {
		PRINTF("---------------------------------------------------------------\n");
		PRINTF("------------------------ OnBoot Menue -------------------------\n");
		PRINTF("---------------------------------------------------------------\n");	
		PRINTF("[ECHO_P] a : Toogle 0\n");
		PRINTF("[ECHO_P] b : Toogle 1\n");
		PRINTF("[ECHO_P] c : Boot Kernel from MMC 0 (override micom config)\n");
		PRINTF("[ECHO_P] d : Boot Kernel from MMC 1 (override micom config)\n");
		PRINTF("[ECHO_P] e : Boot Kernel from MMC 0 (rootfs /dev/mmcblk0p13 )\n");
		PRINTF("[ECHO_P] f : Boot Kernel from MMC 1 (rootfs /dev/mmcblk0p14 )\n");
		PRINTF("[ECHO_P] g : Boot Kernel from MMC 0 (rootfs /dev/sda1 )\n");
		PRINTF("[ECHO_P] h : Boot Kernel from MMC 1 (rootfs /dev/sda1 )\n");		
		PRINTF("[ECHO_P] i : Boot Kernel from MMC 0 (rootfs /dev/nfs )\n");
		PRINTF("[ECHO_P] j : Boot Kernel from MMC 1 (rootfs /dev/nfs )\n");	
		PRINTF("[ECHO_P] k : Boot U-Boot from MMC\n");
		PRINTF("[ECHO_P] l : Reboot\n");
		PRINTF("[ECHO_P] m : Shutdown\n");
		PRINTF("[ECHO_P] n : Serial Download\n");
		PRINTF("---------------------------------------------------------------\n");
		PRINTF("[ECHO_P] x : Continue booting\n");
		PRINTF("---------------------------------------------------------------\n");
		// //markKernel_ext3
		// watchdog 0x17 ??
		while(1)
		{
			rc = 0;
			rc = serial_getc_check(200000);
			if (rc != 0) {
				if (rc == 65 || rc == 97 ) { // a
					micom_cmd(0x35,0, 0); 
					PRINTF("[ECHO_P] Toogle to parition 0 sended\n");
				} else if ( rc == 66 || rc == 98) { // b
					micom_cmd(0x35, 0, 1); 
					PRINTF("[ECHO_P] Toogle to parition 1 sended\n");
				} else if ( rc == 67 || rc == 99) { // c	
					part_flag = 0;	
					PRINTF("[ECHO_P] Continue Booting from parition %d\n", part_flag);
					FastLogo();

					if(part_flag == 0)
					{
						markKernelCMD(PARTITION_ID_KERNEL0);
						_SelectBootMode(PARTITION_ID_KERNEL0);
					}
					else
					{
						markKernelCMD(PARTITION_ID_KERNEL1);
						_SelectBootMode(PARTITION_ID_KERNEL1);
				
					}
					break;
				} else if ( rc == 68 || rc == 100) { // d
					part_flag = 1;	
					PRINTF("[ECHO_P] Continue Booting from parition %d\n", part_flag);
					FastLogo();

					if(part_flag == 0)
					{
						markKernelCMD(PARTITION_ID_KERNEL0);
						_SelectBootMode(PARTITION_ID_KERNEL0);
					}
					else
					{
						markKernelCMD(PARTITION_ID_KERNEL1);
						_SelectBootMode(PARTITION_ID_KERNEL1);
				
					}
					break;
				} else if ( rc == 69 || rc == 101) { // e
					part_flag = 0;	
					PRINTF("[ECHO_P] Continue Booting from parition %d rootfs: /dev/mmcblk0p13 \n", part_flag);
					FastLogo();
				
					if(part_flag == 0)
					{
						markKernel_ext3(PARTITION_ID_KERNEL0);
						_SelectBootMode(PARTITION_ID_KERNEL0);
					}
					else
					{
						markKernel_ext3(PARTITION_ID_KERNEL1);
						_SelectBootMode(PARTITION_ID_KERNEL1);
				
					}
					break;
				} else if ( rc == 70 || rc == 102) { // f
					part_flag = 1;	
					PRINTF("[ECHO_P] Continue Booting from parition %d rootfs:/dev/mmcblk0p14 \n", part_flag);
					FastLogo();
					if(part_flag == 0)
					{
						markKernel_ext3(PARTITION_ID_KERNEL0);
						_SelectBootMode(PARTITION_ID_KERNEL0);
					}
					else
					{
						markKernel_ext3(PARTITION_ID_KERNEL1);
						_SelectBootMode(PARTITION_ID_KERNEL1);
				
					}
					break;				
				} else if ( rc == 71 || rc == 103) { // g	
					// static inline void markKernel_sda1(int flag)
					// static inline void markKernel_nfs(int flag)
					
					PRINTF("[ECHO_P] Booting U-Boot\n");
					_SelectBootMode(PARTITION_ID_U_BOOT);		
				} else if ( rc == 72 || rc == 104) { // h
					PRINTF("[ECHO_P] Rebooting.....\n");
					micom_cmd(0x1d, 0, 0); 
				} else if ( rc == 73 || rc == 105) { // i
					PRINTF("[ECHO_P] Shuting down.....\n");
					micom_cmd(0x12, 0, 0); 
					break;
				} else if ( rc == 74 || rc == 106) { // j
					//PRINTF("[ECHO_P] Serial Download\n");
					//if ( check_host() ) {
					//	int file_size = receive_data();
					//	PRINTF("[ECHO_P] Serial Download %x bytes\n", file_size);
					//} else {
					//	PRINTF("[ECHO_P] Skip Serial Download\n");
					//}
					break;
				} else if ( rc == 88 || rc == 120) { // x	
					PRINTF("[ECHO_P] Continue Booting from parition %d\n", part_flag);
					FastLogo();

					if(part_flag == 0)
					{
						markKernelCMD(PARTITION_ID_KERNEL0);
						_SelectBootMode(PARTITION_ID_KERNEL0);
					}
					else
					{
						markKernelCMD(PARTITION_ID_KERNEL1);
						_SelectBootMode(PARTITION_ID_KERNEL1);
				
					}
					break;
				}
			}
		}
	} else {
	//	if ( check_host() ) {
	//		int file_size = receive_data();
	//		PRINTF("[ECHO_P] Serial Download %x bytes\n", file_size);
	//	} else {
	//		PRINTF("[ECHO_P] Skip Serial Download\n");
	//	}
	}
//#endif
	_SelectBootMode(PARTITION_ID_U_BOOT);	
	// we never come here....
	FastLogo();

	if(part_flag == 0)
	{
		markKernelCMD(PARTITION_ID_KERNEL0);
		_SelectBootMode(PARTITION_ID_KERNEL0);
	}
	else
	{
		markKernelCMD(PARTITION_ID_KERNEL1);
		_SelectBootMode(PARTITION_ID_KERNEL1);

	}

	while (1);    
}
