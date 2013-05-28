#ifndef __CONFIG_H
#define __CONFIG_H

#define getc serial_getc

//#define FREQ_ARM	1000
#define DDRA_BASE_ADDR 0x40000000

#define OS_ONBOOT_ADDR          (DDRA_BASE_ADDR+0x07000000)
#define OS_ONBOOT_ADDR2         (DDRA_BASE_ADDR+0x07100000)
#define OS_UBOOT_ADDR           (DDRA_BASE_ADDR+0x07C00000)
#define OS_KERNEL_ADDR          (DDRA_BASE_ADDR+0x00008000)
#define OS_ROOTFS_ADDR          (DDRA_BASE_ADDR+0x08000000)

#define CMDLINE_TAG_ADDR        (DDRA_BASE_ADDR+0x00000100)
#define MEM_TAG_ADDR            (DDRA_BASE_ADDR+0x00000A00)
#define MMC_EXT_CSD_ADDR        (DDRA_BASE_ADDR+0x05000000) /* 512Byte */
#define MMC_HIGH_CAPA_TAG_ADDR  (DDRA_BASE_ADDR+0x06000000) 

//TODO
#define PARTITION_ID_ONBOOT     0
#define PARTITION_ID_U_BOOT     1
#define PARTITION_ID_KERNEL0    4
#define PARTITION_ID_KERNEL1    6

/* MMC FLASH MAP - LBA*/
#define PRIMARY_PART_SIZE       0x00000010
#define U_BOOT_SIZE             0x00000400
#define KERNEL_1_SIZE           0x00003800
#define RFS_1_SIZE              0x00002c00
#define EXT_PART_SIZE           0x00000010
#define KERNEL_2_SIZE           0x00003800
#define RFS_2_SIZE              0x00002c00
#define CMAC_1_SIZE             0x00000200
#define CMAC_2_SIZE             0x00000200

#define U_BOOT                  (PRIMARY_PART_SIZE)
#define KERNEL_1                (U_BOOT + U_BOOT_SIZE)
#define RFS_1                   (KERNEL_1 + KERNEL_1_SIZE)
#define KERNEL_2                (RFS_1 + RFS_1_SIZE + EXT_PART_SIZE)
#define RFS_2                   (KERNEL_2 + KERNEL_2_SIZE + EXT_PART_SIZE)
#define CMAC_1                  (RFS_2 + RFS_2_SIZE + EXT_PART_SIZE)
#define CMAC_2                  (CMAC_1 + CMAC_1_SIZE + EXT_PART_SIZE)

/* uboot initital Option */
#define CONFIG_ECHO_P
#define CONFIG_SKIP_RELOCATE_UBOOT
#define CONFIG_SDP_NAND		
#define CONFIG_L2_OFF
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_LONGHELP


/* memory environment */
#define CONFIG_SYS_MALLOC_LEN	0x80000
#define CONFIG_SYS_GBL_DATA_SIZE	512

/* UART Initialization */
#define CONFIG_BAUDRATE 115200
#define CONFIG_SERIAL1
#define CONFIG_USE_UART_FIFO	

#define CONFIG_SYS_MAXARGS	0xff
#define CONFIG_SYS_LOAD_ADDR	(DDRA_BASE_ADDR + 0x1000000)
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }
#define CONFIG_SYS_PBSIZE	512
#define CONFIG_SYS_CBSIZE	256
#define CONFIG_SYS_BARGSIZE	256
#define CONFIG_SYS_HZ		FREQ_ARM*1000000


#define CONFIG_SYS_PROMPT	"ECHO-P # "


#define CONFIG_CMD_ENV
#define CONFIG_CMD_LOADS
#define CONFIG_CMD_RUN
#define CONFIG_CMD_BOOTD
#define CONFIG_CMD_SAVEENV
#define CONFIG_CMD_EDITENV
#define CONFIG_CMD_ECHO
#define CONFIG_CMD_MEMORY

#define CONFIG_CMD_FAT
#define CONFIG_CMD_USB
#define CFG_ENV_SIZE			0x00004000		/* 16KB */
#define ENV_SIZE (CFG_ENV_SIZE)

/***********************************************************
 * Buffer options
 ***********************************************************/
/* Console I/O Buffer Size	*/
#define	CFG_CBSIZE			256

/* Print Buffer Size */
#define	CFG_PBSIZE 			(CFG_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)

/* max number of command args	*/
#define	CFG_MAXARGS			16

/* Boot Argument Buffer Size	*/
#define CFG_BARGSIZE			CFG_CBSIZE

/* for SDP board */
#define CONFIG_SDP_MEMTAG

/* Realine Option */
#define  CONFIG_CMDLINE_EDITING
#define  CONFIG_CMDLINE_TAG

#undef CONFIG_SETUP_MEMORY_TAGS
#undef CONFIG_INITRD_TAG

#define CONFIG_AUTO_COMPLETE

#define CONFIG_STACKSIZE	0x20000 	/* 128KB */

#define CONFIG_BOOTDELAY	3

/* Memory */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		(DDRA_BASE_ADDR + 0x0000000)
#define PHYS_SDRAM_1_SIZE	(DDRA_BASE_ADDR + 0x08000000)	/* 128 MB */

#define CONFIG_SYS_SDRAM_BASE	PHYS_SDRAM_1

/* memtest area */
#define CFG_MEMTEST_START	(DDRA_BASE_ADDR + 0x0000000)
#define CFG_MEMTEST_END 	(DDRA_BASE_ADDR + 0x0100000)

/* for CONFIG_CMD_MEMORY */
#define CONFIG_SYS_MEMTEST_START CFG_MEMTEST_START
#define CONFIG_SYS_MEMTEST_END CFG_MEMTEST_END
/* platform dependent misc function */
/* hci_tx_init */
#define CONFIG_MISC_INIT_R
/* Ethernet MAC */
//#define CONFIG_SDP_MAC_RGMII_INTERFACE  // if using rmii, please un-define this line

/* USB */
#ifdef CONFIG_OHCI
#define CONFIG_USB_OHCI_NEW
#define CONFIG_SYS_USB_OHCI_REGS_BASE		0x30078000
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1
#define CONFIG_SYS_USB_OHCI_SLOT_NAME		"SDP OHCI"
#else
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_SDP
#endif
#define CONFIG_USB_STORAGE
#define CONFIG_DOS_PARTITION
#undef CONFIG_USB_STORAGE_ALL   // MS. PARK 07.22

/*************************/
#if (defined(CONFIG_SDP_MMC_OP) && defined(CONFIG_NAND_U_BOOT))
#error CONFIG_SDP_MMC_OP and CONFIG_NAND_U_BOOT is not both setting.





/* for MMC Booting defines ********************************************/
#elif defined(CONFIG_SDP_MMC_OP)

//#define CONFIG_DOS_PARTITION		1 /* mmc */
#define BOOT_PARTITION_BOOT_ADDR	0x0
#define UBOOT_SIZE		0x80000
#define DDR_BOOT_ADDR		TEXT_BASE

#define CONFIG_CMD_MMC

/* MMC Controller Setting */
//#define CONFIG_SDP_MMC_OP /* enable mmc boot in Makefile */
#define CONFIG_GENERIC_MMC /* enable mmc */
#define CONFIG_MMC			1 /* enalble mmc */
#define CONFIG_SDP_MMC 1
#define CONFIG_SYS_MMC_ENV_DEV		0

#define SDP_MMC_DESC_NUM	5120
#define SDP_MMC_DESC_MAX_DATA_SIZE 4096
#define SDP_MMC_USE_DMA
#define SDP_MMC_DESC_ADDR (DDRA_BASE_ADDR + 0x7d00000)
#define SDP_MMC_AUTO_STOP

#define CONFIG_ENV_OFFSET	0x80000
#define CONFIG_ENV_SIZE 	0x1000

#define CONFIG_ENV_IS_IN_MMC	1
#undef CONFIG_ENV_IS_NOWHERE	
#define CONFIG_ENV_OVERWRITE	1

#define CONFIG_SDP_MMC_PART_DOS

//#define SDP_MMC_DDRMODE
#else/* CONFIG_SDP_MMC_OP && CONFIG_NAND_U_BOOT */
#error no booting device.(MMC or NAND)
#endif
#define FALSE   0
#define TRUE    1
#define CONFIG_MICOM_UART
#endif /* __CONFIG_H */

