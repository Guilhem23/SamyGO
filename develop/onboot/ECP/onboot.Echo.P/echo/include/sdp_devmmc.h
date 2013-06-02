#ifndef _SDP_DEVMMC_H_
#define _SDP_DEVMMC_H_

#undef __DEBUG_MMC__

#ifdef __DEBUG_MMC__
#define DPRINTF Print
#else
#define DPRINTF
#endif

/*#define*/
//#define	_MMC_AUTO_STOP_

#define MMC_BLK_SIZE	512
#define _IS_RESP_BUSY(u32cmd, u32arg)	\
	(((u32cmd&0x3F)==CMD12) 	||\
	((u32cmd&0x3F)==CMD28)		||\
	((u32cmd&0x3F)==CMD29)		||\
	((u32cmd&0x3F)==CMD38)		||\
	(((u32cmd&0x3F)==CMD5)	&& (gCard.type==card_type_mmc))|| ((u32cmd&0x3F)==CMD7 && (u32arg==(gCard.RCA<<16)))||	(((u32cmd&0x3F)==CMD6) && (gCard.type==card_type_mmc)))
	
#define MMC_CARD_BUSY	0x80000000	/* Card Power up status bit */
#define MMC_CARD_CCS	0x40000000	/* Card Capacity Status bit */
#define ERR_TIMED_OUT		(1<<16) //bit1-15 RINTSTS err bits
#define ERR_MAX_RETRY		(1<<17)
#define DEFAULT_DEBNCE_VAL		   0x0FFFFF
#define GET_R6_RCA(x)		(((x)&0xffff0000)>>16)
#define GET_FIFO_DEPTH(x)		((((x)&0x0FFF0000)>>16)+1)

#define SDP_MMC_UPDATE_CLOCK			\
			SDP_MMC_CMD_START_CMD | SDP_MMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY | SDP_MMC_CMD_WARVDATA_COMPLETE

#define MMC_VDD_165_195 	0x00000080	/* VDD voltage 1.65 - 1.95 */



typedef void (*sdp_mmc_preproc_callback)(u32, u32, u32*, u32*);
typedef void (*sdp_mmc_postproc_callback)(void *, u32 *);

typedef struct _RegInfoMmcif_s {
	char *RegName;
	u32 RegAddr;
	u32 RegMask;
	u32 Reset;
} RegInfoMmcif_t;



typedef struct {
	u32 cmd_index;
	sdp_mmc_preproc_callback preproc;
	sdp_mmc_postproc_callback postproc;
}callback_search_table;

typedef struct{

	u32	mmc_reserved1	: 1,
			mmc_dic 		: 1,
			mmc_ld			: 1,
			mmc_fs			: 1,
			mmc_ch			: 1,
			mmc_er			: 1,
			mmc_reserved2	: 24,
			mmc_ces 		: 1,
			mmc_own 		: 1;
			

} mmc_des0_struct;

typedef struct{

	u32	mmc_buffer1_size	: 13,
			mmc_buffer2_size	: 13,
			mmc_reserved1	: 6;

} mmc_des1_struct;
typedef struct{

	u32	buffer_address_pointer1 : 32;

} mmc_des2_struct;
typedef struct{

	u32	buffer_address_pointer2 : 32;

} mmc_des3_struct;

typedef union{
	u32 des0;
	mmc_des0_struct des0_t;
} mmc_des0_union;
typedef union{
	u32 des1;
	mmc_des1_struct des1_t;
} mmc_des1_union;
typedef union{
	u32 des2;
	mmc_des2_struct des2_t;
} mmc_des2_union;
typedef union{
	u32 des3;
	mmc_des3_struct des3_t;
} mmc_des3_union;

typedef struct{
	mmc_des0_union mmc_des0;
	mmc_des1_union mmc_des1;
	mmc_des2_union mmc_des2;
	mmc_des3_union mmc_des3;
}	mmc_descriptor;

#define MAX_NUM_SG			7484 //Rudra
/*This has 13bits and must be multiple of 4(for 32-bit bus), hence 8188*/	
/*7680 because multi block can write N*(512). [Max value of N*512<8188=7680]*/
#define SG_MAX_SIZE 	7680 //15*512
#define SDP_MMC_MIN_CLOCK			400000		// 400khz	specification 0 ~ 400Khz
#define SDP_MMC_PROCESSOR_CLK	(100000000)
#define SDP_MMC_MAX_CLK		 50000000

#define CLK_SPEED_MIN		(0xFF)
#define CLK_HIGH_SPEED		(0) 	/*Freq=PCLK/2*CLKDIV*/
#define SEL 				1
#define DESEL				2
#define SDP_MMC_SUCCESS	0
#define SDP_MMC_ERROR		1
#define ARG_HS_SET_VALUE	0x00000100
#define ARG_EXTCSD_ACCESS_SET	0x03000000
#define ARG_HS_INDEX		0x00b90000
#define ARG_CMD_SET_INDEX	0x00bf0000
#define ARG_EXTCSD_IDX_POS		16
#define ARG_EXTCSD_VAL_POS	8

#define ARG_BUS_WIDTH_INDEX 0x00b70000
#define ARG_8_BIT_WIDTH 	0x00000200
#define ARG_4_BIT_WIDTH 	0x00000100

#define SDP_MMC_DDR_52MHZ_12V	0x8
#define SDP_MMC_DDR_52MHZ_18_3V 0x4

#define READ_MMC_MODE 1
#define WRITE_MMC_MODE	2

#define SDP_MMC_CMD0_ERR 0x100
#define SDP_MMC_CMD1_ERR 0x101
#define SDP_MMC_CMD2_ERR 0x102
#define SDP_MMC_CMD3_ERR 0x103
#define SDP_MMC_CMD4_ERR 0x104
#define SDP_MMC_CMD5_ERR 0x105
#define SDP_MMC_CMD6_ERR 0x106
#define SDP_MMC_CMD7_ERR 0x107
#define SDP_MMC_CMD8_ERR 0x108
#define SDP_MMC_CMD9_ERR 0x109
#define SDP_MMC_CMD12_ERR 0x1012
#define SDP_MMC_CMD13_ERR 0x1013
#define SDP_MMC_CMD16_ERR 0x1016
#define SDP_MMC_CMD17_ERR 0x1017
#define SDP_MMC_CMD18_ERR 0x1018
#define SDP_MMC_CMD24_ERR 0x1024
#define SDP_MMC_CMD25_ERR 0x1025



#if 0
#define UNSTUFF_BITS(resp,start,size)					\
	({								\
		const int __size = size;				\
		const u32 __mask = (__size < 32 ? 1 << __size : 0) - 1;	\
		const int __off = 3 - ((start) / 32);			\
		const int __shft = (start) & 31;			\
		u32 __res;						\
									\
		__res = resp[__off] >> __shft;				\
		if (__size + __shft > 32)				\
			__res |= resp[__off-1] << ((32 - __shft) % 32); \
		__res & __mask; 					\
	})
#endif	
typedef enum {
	card_state_idle=0,
	card_state_ready,
	card_state_ident,
	card_state_stby,
	card_state_trans,
	card_state_data,
	card_state_rcv,
	card_state_prg,
	card_state_dis, 
}enCard_state_t;

typedef enum {
	card_type_sd=1,
	card_type_sdhc, 
	card_type_mmc,	
	card_type_sdio, 	
}enCard_type_t;

typedef struct CardInfo{
	/*Card's Init Condition. Set this after Card is through ready state*/
	u32 init;
	
	/*Card's Internal state*/
	u32 state;
	
	/*Card's Relative Addr*/
	u32 RCA;
	
	/*Card Specific Data*/
	u32 CSD[4];
	
	/*Card Identification Number*/
	u32 CID[4];

	/*SCR Register*/
	u32 SCR[2];
	
	/*Card Type*/
	u32 type;

	/*Current block length*/	
	u32 u32blklen;	
} CardInfo_t;

struct sdp_mmc_cid {
	unsigned int		manfid;
	char			prod_name[8];
	unsigned int		serial;
	unsigned short		oemid;
	unsigned short		year;
	unsigned char		hwrev;
	unsigned char		fwrev;
	unsigned char		month;
};

struct sdp_mmc_csd {
	unsigned char		mmca_vsn;
	unsigned short		cmdclass;
	unsigned short		tacc_clks;
	unsigned int		tacc_ns;
	unsigned int		r2w_factor;
	unsigned int		max_dtr;
	unsigned int		read_blkbits;
	unsigned int		write_blkbits;
	unsigned int		capacity;
	unsigned int		read_partial:1,
				read_misalign:1,
				write_partial:1,
				write_misalign:1;
};
struct sd_scr {
	unsigned char		sda_vsn;
	unsigned char		bus_widths;
#define SD_SCR_BUS_WIDTH_1	(1<<0)
#define SD_SCR_BUS_WIDTH_4	(1<<2)
};

/* locking/unlocking of SD */
typedef struct {
	u8 lock_cmd_type;
	u8 passwd_length;
	u8 passwd[32];	
} Lock_Struct;

typedef struct {
	u32 desc0;
	u32 desc1;
	u32 desc2;
	u32 desc3;
} sdp_mmc_desc;

enum DmaDescriptorDES0	  // Control and status word of DMA descriptor DES0 
{
	 DescOwnByDma		   = 0x80000000,   /* (OWN)Descriptor is owned by DMA engine			  31   */
	 DescCardErrSummary    = 0x40000000,   /* Indicates EBE/RTO/RCRC/SBE/DRTO/DCRC/RE			  30   */
	 DescEndOfRing		   = 0x00000020,   /* A "1" indicates End of Ring for Ring Mode 		  05   */
	 DescSecAddrChained    = 0x00000010,   /* A "1" indicates DES3 contains Next Desc Address	  04   */
	 DescFirstDesc		   = 0x00000008,   /* A "1" indicates this Desc contains first			  03
											  buffer of the data									   */
	 DescLastDesc		   = 0x00000004,   /* A "1" indicates buffer pointed to by this this	  02
											  Desc contains last buffer of Data 					   */
	 DescDisInt 		   = 0x00000002,   /* A "1" in this field disables the RI/TI of IDSTS	  01
											  for data that ends in the buffer pointed to by 
											  this descriptor										   */	  
};

enum DmaDescriptorDES1	  // Buffer's size field of Descriptor
{
	 DescBuf2SizMsk 	  = 0x03FFE000,    /* Mask for Buffer2 Size 						   25:13   */
	 DescBuf2SizeShift	  = 13, 		   /* Shift value for Buffer2 Size							   */
	 DescBuf1SizMsk 	  = 0x00001FFF,    /* Mask for Buffer1 Size 						   12:0    */
	 DescBuf1SizeShift	  = 0,			   /* Shift value for Buffer2 Size							   */
};

u32 mmc_desc_init(u32 desc_addr, u32 data_addr, u32 data_size);
void sdp_mmc_init_controller(void);
void mmc_plat_loop(u32 counter);
int mmc_poll_cmd_done(void);
void _mmc_wait_on_data_state_mc_busy(u32 timeout);
void _mmc_wait_on_busy(u32 timeout);
void _mmc_wait_for_intr(u32 timeout);
void _mmc_wait_for_dto(u32 timeout);
int _mmc_get_err(void);
void mmc_response_status(void);
int mmc_send_cmd_wait(u32 u32cmd, u32 u32arg);
int sdp_mmc_set_clk(u32 clock);
int sdp_mmc_go_idle_state(void);
int sdp_mmc_sd_send_if_cond(void);
int sdp_mmc_send_op_cond(void);
int sdp_mmc_all_send_cid(void);
int sdp_mmc_send_relative_addr(void);
int sdp_mmc_send_csd(void);
u32 sdp_mmc_select_card(u32 select);
int sdp_mmc_send_ext_csd(u8 *ext_csd);
u32 sdp_mmc_switch_ext_csd(u8 access, u8 index, u8 value, u8 cmdset);
int sdp_mmc_switch(u8 access, u8 index, u8 value, u8 cmdset);
int sdp_mmc_set_block_length(u32 u32blklen);
#ifndef SDP_MMC_USE_DMA
void sdp_mmc_do_pio(u32 read, u8 *pui8buff, u32 u32size);
#endif
int sdp_mmc_read_write(u32 read, u32 u32addr, u8 *pui8buff, u32 u32size_in_bytes);
int sdp_mmc_RW(u32 bread, u32 card_addr, u32 ram_addr, u32 u32size);


#endif
