#ifndef SDP_MMC_H
#define SDP_MMC_H
#define RegRead(Addr)		(*(volatile unsigned int*)(Addr))
#define RegWrite(Addr, data)	(*(volatile unsigned int*)(Addr) = data)


/* Standard MMC commands (3.1)			 type  argument 	response */
   /* class 1 */
#define CMD0	0	/* MMC_GO_IDLE_STATE		bc					  */
#define CMD1	1	/* MMC_SEND_OP_COND 		bcr  [31:0]  OCR  R3  */
#define CMD2	2	/* MMC_ALL_SEND_CID 		bcr 			  R2  */
#define CMD3	3	/* MMC_SET_RELATIVE_ADDR	ac	 [31:16] RCA  R1  */
#define CMD4	4	/* MMC_SET_DSR				bc	 [31:16] RCA	  */

#define CMD5	5	/* SDIO_SEND_OCR			??	 ?? 			  */

#define CMD6	6	/* HSMMC_SWITCH 			ac				  R1  */
					/* For ACMD6:SET_BUS_WIDTH	??	 ?? 			  */

#define CMD7	7	/* MMC_SELECT_CARD			ac	 [31:16] RCA  R1  */
#define CMD8	8	/* HSMMC_SEND_EXT_CSD		adtc [31:16] RCA  R1  */
#define CMD9	9	/* MMC_SEND_CSD 			ac	 [31:16] RCA  R2  */
#define CMD10	10	/* MMC_SEND_CID 			ac	 [31:16] RCA  R2  */
#define CMD11	11	/* MMC_READ_DAT_UNTIL_STOP	adtc [31:0]  dadr R1  */
#define CMD12	12	/* MMC_STOP_TRANSMISSION	ac				  R1b */
#define CMD13	13	/* MMC_SEND_STATUS			ac	 [31:16] RCA  R1  */
#define ACMD13	13	/* SD_STATUS				ac	 [31:2] Stuff,
													 [1:0]Buswidth	R1*/
#define CMD14	14	/* HSMMC_BUS_TESTING		adtc [31:16] stuff R1 */
#define CMD15	15	/* MMC_GO_INACTIVE_STATE	ac	 [31:16] RCA  */
#define CMD19	19	/* HSMMC_BUS_TESTING		adtc [31:16] stuff R1 */

  /* class 2 */
#define CMD16	16	/* MMC_SET_BLOCKLEN 		ac	 [31:0] blkln R1  */
#define CMD17	17	/* MMC_READ_SINGLE_BLOCK	adtc [31:0] dtadd R1  */
#define CMD18	18	/* MMC_READ_MULTIPLE_BLOCK	adtc [31:0] dtadd R1  */

  /* class 3 */
#define CMD20	20	/* MMC_WRITE_DAT_UNTIL_STOP adtc [31:0] dtadd R1  */

  /* class 4 */
#define CMD23	23	/* MMC_SET_BLOCK_COUNT		adtc [31:0] dtadd R1  */
#define CMD24	24	/* MMC_WRITE_BLOCK			adtc [31:0] dtadd R1  */
#define CMD25	25	/* MMC_WRITE_MULTIPLE_BLOCK adtc			  R1  */
#define CMD26	26	/* MMC_PROGRAM_CID			adtc			  R1  */
#define CMD27	27	/* MMC_PROGRAM_CSD			adtc			  R1  */

  /* class 6 */
#define CMD28	28	/* MMC_SET_WRITE_PROT		ac	 [31:0] dtadd R1b */
#define CMD29	29	/* _CLR_WRITE_PROT			ac	 [31:0] dtadd R1b */
#define CMD30	30	/* MMC_SEND_WRITE_PROT		adtc [31:0] wpdtaddr R1  */

  /* class 5 */
#define CMD32	32	/* SD_ERASE_GROUP_START    ac	[31:0] dtadd  R1  */
#define CMD33	33	/* SD_ERASE_GROUP_END	   ac	[31:0] dtaddr R1  */

#define CMD35	35	/* MMC_ERASE_GROUP_START	ac	 [31:0] dtadd  R1  */
#define CMD36	36	/* MMC_ERASE_GROUP_END		ac	 [31:0] dtaddr R1  */
#define CMD38	38	/* MMC_ERASE				ac				   R1b */

  /* class 9 */
#define CMD39	39	/* MMC_FAST_IO				ac	 <Complex>	   R4  */
#define CMD40	40	/* MMC_GO_IRQ_STATE 		bcr 			   R5  */

#define ACMD41	41	/* SD_SEND_OP_COND			??				   R1  */

  /* class 7 */
#define CMD42	42	/* MMC_LOCK_UNLOCK			adtc			   R1b */

#define ACMD51	51	/* SEND_SCR 				adtc			   R1  */

#define CMD52	52	/* SDIO_RW_DIRECT			??				   R5  */
#define CMD53	53	/* SDIO_RW_EXTENDED 		??				   R5  */

  /* class 8 */
#define CMD55	55	/* MMC_APP_CMD				ac	 [31:16] RCA   R1  */
#define CMD56	56	/* MMC_GEN_CMD				adtc [0] RD/WR	   R1b */


#define CMD60	60
#define CMD61	61
#define CMD62	62

//Rudra
#define ACMD22	22
#define ACMD6	6

#define SDP_MMC_BASE_ADDR 		0x30000000


#define SDP_MMC_REG_CTRL			(SDP_MMC_BASE_ADDR+0x0)	/** Control */
#define SDP_MMC_REG_PWREN 	(SDP_MMC_BASE_ADDR+0x4)	/** Power-enable */
#define SDP_MMC_REG_CLKDIV		(SDP_MMC_BASE_ADDR+0x8)	/** Clock divider */
#define SDP_MMC_REG_CLKSRC		(SDP_MMC_BASE_ADDR+0xC)	/** Clock source */
#define SDP_MMC_REG_CLKENA		(SDP_MMC_BASE_ADDR+0x10)	/** Clock enable */
#define SDP_MMC_REG_TMOUT 	(SDP_MMC_BASE_ADDR+0x14)	/** Timeout */
#define SDP_MMC_REG_CTYPE 	(SDP_MMC_BASE_ADDR+0x18)	/** Card type */
#define SDP_MMC_REG_BLKSIZ		(SDP_MMC_BASE_ADDR+0x1C)	/** Block Size */
#define SDP_MMC_REG_BYTCNT		(SDP_MMC_BASE_ADDR+0x20)	/** Byte count */
#define SDP_MMC_REG_INTMSK		(SDP_MMC_BASE_ADDR+0x24)	/** Interrupt Mask */
#define SDP_MMC_REG_CMDARG		(SDP_MMC_BASE_ADDR+0x28)	/** Command Argument */
#define SDP_MMC_REG_CMD			(SDP_MMC_BASE_ADDR+0x2C)	/** Command */
#define SDP_MMC_REG_RESP0 	(SDP_MMC_BASE_ADDR+0x30)	/** Response 0 */
#define SDP_MMC_REG_RESP1 	(SDP_MMC_BASE_ADDR+0x34)	/** Response 1 */
#define SDP_MMC_REG_RESP2 	(SDP_MMC_BASE_ADDR+0x38)	/** Response 2 */
#define SDP_MMC_REG_RESP3 	(SDP_MMC_BASE_ADDR+0x3C)	/** Response 3 */
#define SDP_MMC_REG_MINTSTS		(SDP_MMC_BASE_ADDR+0x40)	/** Masked interrupt status */
#define SDP_MMC_REG_RINTSTS		(SDP_MMC_BASE_ADDR+0x44)	/** Raw interrupt status */
#define SDP_MMC_REG_STATUS		(SDP_MMC_BASE_ADDR+0x48)	/** Status */
#define SDP_MMC_REG_FIFOTH		(SDP_MMC_BASE_ADDR+0x4C)	/** FIFO threshold */
#define SDP_MMC_REG_CDETECT		(SDP_MMC_BASE_ADDR+0x50)	/** Card detect */
#define SDP_MMC_REG_WRTPRT		(SDP_MMC_BASE_ADDR+0x54)	/** Write protect */
#define SDP_MMC_REG_GPIO			(SDP_MMC_BASE_ADDR+0x58)	/** General Purpose IO */
#define SDP_MMC_REG_TCBCNT		(SDP_MMC_BASE_ADDR+0x5C)	/** Transferred CIU byte count */
#define SDP_MMC_REG_TBBCNT		(SDP_MMC_BASE_ADDR+0x60)	/** Transferred host/DMA to/from byte count */
#define SDP_MMC_REG_DEBNCE		(SDP_MMC_BASE_ADDR+0x64)	/** Card detect debounce */
#define SDP_MMC_REG_USRID 	(SDP_MMC_BASE_ADDR+0x68)	/** User ID */
#define SDP_MMC_REG_VERID 	(SDP_MMC_BASE_ADDR+0x6C)	/** Version ID */
#define SDP_MMC_REG_HCON		(SDP_MMC_BASE_ADDR+0x70)	/** Hardware Configuration */
#define SDP_MMC_REG_UHS 	(SDP_MMC_BASE_ADDR+0x74)	/** UHS-1 */
#define SDP_MMC_REG_RST 	(SDP_MMC_BASE_ADDR+0x78)	/** H/W Reset */

#define SDP_MMC_REG_BMOD		(SDP_MMC_BASE_ADDR+0x80)	/** Bus Mode Register */
#define SDP_MMC_REG_PLDMND		(SDP_MMC_BASE_ADDR+0x84)	/** Poll Demand Register */
#define SDP_MMC_REG_DBADDR		(SDP_MMC_BASE_ADDR+0x88)	/** Descriptor List Base Address Register */
#define SDP_MMC_REG_IDSTS 	(SDP_MMC_BASE_ADDR+0x8C)	/** Internal DMAC Status Register */
#define SDP_MMC_REG_IDINTEN		(SDP_MMC_BASE_ADDR+0x90)	/** Internal DMAC Interrupt Enable Register */
#define SDP_MMC_REG_DSCADDR		(SDP_MMC_BASE_ADDR+0x94)	/** Current Host Descriptor Address Register */
#define SDP_MMC_REG_BUFADDR		(SDP_MMC_BASE_ADDR+0x98)	/** Current Host Buffer Address Register */
#define SDP_MMC_REG_FIFODAT		(SDP_MMC_BASE_ADDR+0x100) /** FIFO data read write */

//Rudra
/*STATUS Register definitions*/
#define SDP_MMC_STATUS_FIFO_EMPTY (0x1<<2)
#define SDP_MMC_STATUS_FIFO_FULL	(0x1<<3)
#define SDP_MMC_STATUS_DAT_BUSY		(0x00000200)

/* Control register definitions */
#define SDP_MMC_CTRL_USE_INTERNAL_DMAC					(0x1<<25)
#define SDP_MMC_CTRL_ENABLE_OD_PULLUP 				(0x1<<24)
#define SDP_MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS	(0x1<<11)
#define SDP_MMC_CTRL_SEND_AUTO_STOP_CCSD				(0x1<<10)
#define SDP_MMC_CTRL_SEND_CCSD							(0x1<<9)
#define SDP_MMC_CTRL_ABORT_READ_DATA					(0x1<<8)
#define SDP_MMC_CTRL_SEND_IRQ_RESPONSE					(0x1<<7)
#define SDP_MMC_CTRL_READ_WAIT							(0x1<<6)
#define SDP_MMC_CTRL_DMA_ENABLE							(0x1<<5)
#define SDP_MMC_CTRL_INT_ENABLE							(0x1<<4)
#define SDP_MMC_CTRL_DMA_RESET							(0x1<<2)
#define SDP_MMC_CTRL_FIFO_RESET							(0x1<<1)
#define SDP_MMC_CTRL_CONTROLLER_RESET 				(0x1<<0)

/* Clock Divider register definitions */
#define SDP_MMC_CLKDIV_3(x)			((x)<<24)
#define SDP_MMC_CLKDIV_2(x)			((x)<<16)
#define SDP_MMC_CLKDIV_1(x)			((x)<<8)
#define SDP_MMC_CLKDIV_0(x)			((x)<<0)

/* Clock Enable register definitions */
#define SDP_MMC_CLKENA_ALL_CCLK_ENABLE			(0xffff)

/* Timeout register definitions */
#define SDP_MMC_TMOUT_DATA_TIMEOUT(x) 		((x)<<8)
#define SDP_MMC_TMOUT_RESPONSE_TIMEOUT(x) 	((x)<<0)

/*CURRENT STATE */
#define SDP_MMC_IDLE		(0x0<<9)
#define SDP_MMC_READY (0x1<<9)
#define SDP_MMC_IDENT 	(0x2<<9)
#define SDP_MMC_STBY		(0x3<<9)
#define SDP_MMC_TRAN		(0x4<<9)
#define SDP_MMC_DATA		(0x5<<9)
#define SDP_MMC_RCV		(0x6<<9)
#define SDP_MMC_PRG		(0x7<<9)
#define SDP_MMC_DIS		(0x8<<9)
#define SDP_MMC_BTST		(0x9<<9)
#define SDP_MMC_SLP		(0xa<<9)


/* Interrupt mask defines */
#define SDP_MMC_INTMSK_EBE			(0x1<<15)
#define SDP_MMC_INTMSK_ACD			(0x1<<14)
#define SDP_MMC_INTMSK_SBE			(0x1<<13)
#define SDP_MMC_INTMSK_HLE			(0x1<<12)
#define SDP_MMC_INTMSK_FRUN			(0x1<<11)
#define SDP_MMC_INTMSK_HTO			(0x1<<10)
#define SDP_MMC_INTMSK_DRTO			(0x1<<9)
#define SDP_MMC_INTMSK_RTO			(0x1<<8)
#define SDP_MMC_INTMSK_DCRC			(0x1<<7)
#define SDP_MMC_INTMSK_RCRC			(0x1<<6)
#define SDP_MMC_INTMSK_RXDR			(0x1<<5)
#define SDP_MMC_INTMSK_TXDR			(0x1<<4)
#define SDP_MMC_INTMSK_DTO			(0x1<<3)
#define SDP_MMC_INTMSK_CMD_DONE	(0x1<<2)
#define SDP_MMC_INTMSK_RE 		(0x1<<1)
#define SDP_MMC_INTMSK_CD 		(0x1<<0)
#define SDP_MMC_INTMSK_SDIO_INTR		(0xffff<<16)
#define SDP_MMC_INTMSK_SDIO_CARD(x)	(0x1<<(16+x))
#define SDP_MMC_INTMSK_ALL_ENABLED	(0xffffffff)

/* Masked Interrupt Status Register defines */
#define SDP_MMC_MINTSTS_EBE				(0x1<<15)
#define SDP_MMC_MINTSTS_ACD				(0x1<<14)
#define SDP_MMC_MINTSTS_SBE				(0x1<<13)
#define SDP_MMC_MINTSTS_HLE				(0x1<<12)
#define SDP_MMC_MINTSTS_FRUN				(0x1<<11)
#define SDP_MMC_MINTSTS_HTO				(0x1<<10)
#define SDP_MMC_MINTSTS_DRTO				(0x1<<9)
#define SDP_MMC_MINTSTS_RTO				(0x1<<8)
#define SDP_MMC_MINTSTS_DCRC				(0x1<<7)
#define SDP_MMC_MINTSTS_RCRC				(0x1<<6)
#define SDP_MMC_MINTSTS_RXDR				(0x1<<5)
#define SDP_MMC_MINTSTS_TXDR				(0x1<<4)
#define SDP_MMC_MINTSTS_DTO				(0x1<<3)
#define SDP_MMC_MINTSTS_CMD_DONE		(0x1<<2)
#define SDP_MMC_MINTSTS_RE				(0x1<<1)
#define SDP_MMC_MINTSTS_CD				(0x1<<0)
#define SDP_MMC_MINTSTS_SDIO_INTR 	(0xffff<<16)
#define SDP_MMC_MINTSTS_SDIO_CARD(x)	(0x1<<(16+x))
#define SDP_MMC_MINTSTS_ALL_ENABLED	(0xffffffff)

/* Raw Interrupt Status Register defines */
#define SDP_MMC_RINTSTS_EBE				(0x1<<15)
#define SDP_MMC_RINTSTS_ACD				(0x1<<14)
#define SDP_MMC_RINTSTS_SBE				(0x1<<13)
#define SDP_MMC_RINTSTS_HLE				(0x1<<12)
#define SDP_MMC_RINTSTS_FRUN				(0x1<<11)
#define SDP_MMC_RINTSTS_HTO				(0x1<<10)
#define SDP_MMC_RINTSTS_DRTO				(0x1<<9)
#define SDP_MMC_RINTSTS_RTO				(0x1<<8)
#define SDP_MMC_RINTSTS_DCRC				(0x1<<7)
#define SDP_MMC_RINTSTS_RCRC				(0x1<<6)
#define SDP_MMC_RINTSTS_RXDR				(0x1<<5)
#define SDP_MMC_RINTSTS_TXDR				(0x1<<4)
#define SDP_MMC_RINTSTS_DTO				(0x1<<3)
#define SDP_MMC_RINTSTS_CMD_DONE		(0x1<<2)
#define SDP_MMC_RINTSTS_RE				(0x1<<1)
#define SDP_MMC_RINTSTS_CD				(0x1<<0)
#define SDP_MMC_RINTSTS_SDIO_INTR 	(0xffff<<16)
#define SDP_MMC_RINTSTS_SDIO_CARD(x)		(0x1<<(16+x))
#define SDP_MMC_RINTSTS_ALL_ENABLED		(0xffffffff)



/* CMD Register Defines */
#define SDP_MMC_CMD_START_CMD 					(0x1<<31)
#define SDP_MMC_CMD_BOOT_MODE 					(0x1<<27)
#define SDP_MMC_CMD_DISABLE_BOOT					(0x1<<26)
#define SDP_MMC_CMD_EXPECT_BOOT_ACK				(0x1<<25)
#define SDP_MMC_CMD_ENABLE_BOOT					(0x1<<24)
#define SDP_MMC_CMD_CCS_EXPECTED					(0x1<<23)
#define SDP_MMC_CMD_READ_CEATA_DEVICE 			(0x1<<22)
#define SDP_MMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY	(0x1<<21)
#define SDP_MMC_CMD_SEND_INITIALIZATION				(0x1<<15)
#define SDP_MMC_CMD_STOP_ABORT_CMD				(0x1<<14)
#define SDP_MMC_CMD_WARVDATA_COMPLETE 		(0x1<<13)
#define SDP_MMC_CMD_SEND_AUTO_STOP					(0x1<<12)
#define SDP_MMC_CMD_TRANSFER_MODE 				(0x1<<11)
#define SDP_MMC_CMD_READ_WRITE						(0x1<<10)
#define SDP_MMC_CMD_DATA_EXPECTED 				(0x1<<9)
#define SDP_MMC_CMD_CHECK_RESPONSE_CRC			(0x1<<8)
#define SDP_MMC_CMD_RESPONSE_LENGTH				(0x1<<7)
#define SDP_MMC_CMD_RESPONSE_EXPECT				(0x1<<6)
#define SDP_MMC_CMD_MAX_RETRIES						500

/* Hardware Configuration Register */
#define SDP_MMC_HCON_MMC_ONLY 					(0x0<<0)
#define SDP_MMC_HCON_SD_MMC						(0x1<<0)
#define SDP_MMC_HCON_APB							(0x0<<6)
#define SDP_MMC_HCON_AHB							(0x1<<6)
#define SDP_MMC_HCON_DW_DMA						(0x1<<16)
#define SDP_MMC_HCON_GENERIC_DMA					(0x2<<16)
#define SDP_MMC_HCON_GE_DMA_DATA_WIDTH_16BITS (0x0<<18)
#define SDP_MMC_HCON_GE_DMA_DATA_WIDTH_32BITS (0x1<<18)
#define SDP_MMC_HCON_GE_DMA_DATA_WIDTH_64BITS (0x2<<18)
#define SDP_MMC_HCON_FIFO_RAM_OUTSIDE 			(0x0<<21)
#define SDP_MMC_HCON_FIFO_RAM_INSIDE				(0x1<<21)
#define SDP_MMC_HCON_IMPLEMENT_NO_HOLD_REG		(0x0<<22)
#define SDP_MMC_HCON_IMPLEMENT_HOLD_REG			(0x1<<22)
#define SDP_MMC_HCON_SET_CLK_NO_FALSE_PATH		(0x0<<23)
#define SDP_MMC_HCON_SET_CLK_FALSE_PATH			(0x1<<23)
#define SDP_MMC_HCON_NO_AREA_OPTIMIZED			(0x0<<26)
#define SDP_MMC_HCON_AREA_OPTIMIZED				(0x1<<26)

/* Internal DMAC Status Register */
#define SDP_MMC_IDSTS_FSM_DMA_IDLE					(0x0<<13)
#define SDP_MMC_IDSTS_FSM_DMA_SUSPEND 			(0x1<<13)
#define SDP_MMC_IDSTS_FSM_DESC_RD 				(0x2<<13)
#define SDP_MMC_IDSTS_FSM_DESC_CHK					(0x3<<13)
#define SDP_MMC_IDSTS_FSM_DMA_RD_REQ_WAIT 		(0x4<<13)
#define SDP_MMC_IDSTS_FSM_DMA_WR_REQ_WAIT 	(0x5<<13)
#define SDP_MMC_IDSTS_FSM_DMA_RD					(0x6<<13)
#define SDP_MMC_IDSTS_FSM_DMA_WR					(0x7<<13)
#define SDP_MMC_IDSTS_FSM_DESC_CLOSE				(0x8<<13)
#define SDP_MMC_IDSTS_AIS 							(0x1<<9)
#define SDP_MMC_IDSTS_NIS 							(0x1<<8)
#define SDP_MMC_IDSTS_CES 						(0x1<<5)
#define SDP_MMC_IDSTS_DU								(0x1<<4)
#define SDP_MMC_IDSTS_FBE 						(0x1<<2)
#define SDP_MMC_IDSTS_RI								(0x1<<1)
#define SDP_MMC_IDSTS_TI								(0x1<<0)

/* Card Type Register */
#define SDP_MMC_CTYPE_NON_8BIT_MODE				(0x0<<16)
#define SDP_MMC_CTYPE_8BIT_MODE						(0x1<<16)
#define SDP_MMC_CTYPE_1BIT_MODE						(0x0<<0)
#define SDP_MMC_CTYPE_4BIT_MODE						(0x1<<0)

/* UHS-1 Register */
#define SDP_MMC_UHS_DDR_REG(x)				((x) << 16)
#define SDP_MMC_VOLT_REG(x) 			((x) << 0) //default is 0

/* Bus Mode Register */
#define SDP_MMC_BMOD_PBL_1TRANS					(0x0<<8)
#define SDP_MMC_BMOD_PBL_4TRANS					(0x1<<8)
#define SDP_MMC_BMOD_PBL_8TRANS					(0x2<<8)
#define SDP_MMC_BMOD_PBL_16TRANS					(0x3<<8)
#define SDP_MMC_BMOD_PBL_32TRANS					(0x4<<8)
#define SDP_MMC_BMOD_PBL_64TRANS					(0x5<<8)
#define SDP_MMC_BMOD_PBL_128TRANS 				(0x6<<8)
#define SDP_MMC_BMOD_PBL_256TRANS 				(0x7<<8)
#define SDP_MMC_BMOD_DE								(0x1<<7)
#define SDP_MMC_BMOD_DSL(x)							((x)<<2)
#define SDP_MMC_BMOD_FB								(0x1<<1)
#define SDP_MMC_BMOD_SWR							(0x1<<0)

/* FIFO Threshold Watermark Register */
#define SDP_MMC_FIFOTH_DW_DMA_MTS_1TRANS			(0x0<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_4TRANS			(0x1<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_8TRANS			(0x2<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_16TRANS 	(0x3<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_32TRANS 	(0x4<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_64TRANS 	(0x5<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_128TRANS		(0x6<<28)
#define SDP_MMC_FIFOTH_DW_DMA_MTS_256TRANS		(0x7<<28)
#define SDP_MMC_FIFOTH_RX_WMARK(x)					((x)<<16)
#define SDP_MMC_FIFOTH_TX_WMARK(x)					((x)<<0)

#define SDP_MMC_DES_ADDR						(DDRA_BASE_ADDR + 0x2000000)
#define SDP_MMC_DES_BASE_ADDR 			(DDRA_BASE_ADDR + 0x2000000 + 0x00100000)

#define SDP_MMC_WRITE_BAP_OFFSET			0x00001000
#define SDP_MMC_READ_BAP_OFFSET				0x00005000


/* Operation Conditions Register (OCR) Register Definition */

#define SDP_MMC_OCR_17TO19					0x00000080
#define SDP_MMC_OCR_20TO26					0x00000000
#define SDP_MMC_OCR_27TO36					0x00ff8000
#define SDP_MMC_OCR_BYTE_MODE 			0x00000000
#define SDP_MMC_OCR_SECTOR_MODE			0x40000000
#define SDP_MMC_OCR_POWER_UP_STATUS		0x80000000

/* Set Block Count Definition [CMD23] */
#define SDP_MMC_RELIABLE_WRITE_REQUEST		(0x1<<31)
#define SDP_MMC_SBC_READ_MODE 			1
#define SDP_MMC_SBC_WRITE_MODE				2

#define DEFAULT_DEBNCE_VAL		   0x0FFFFF



#define SDP_MMC_SET_BITS(x,y) 				((x)|=(y))






#endif
