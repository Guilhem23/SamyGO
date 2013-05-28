#include <sp_setup.h>
#include <config.h>
#include <sdp_mmc.h>
#include <sdp_devmmc.h>
#include <mmc.h>

extern int high_capacity;
extern unsigned long _start, __bss_start;
u32 sdp_mmc_init(void){
	int err;
    int ext_csd_sectors;
    u8 * pExt_csd = MMC_EXT_CSD_ADDR;
    
	unsigned int boot_size, chk_size;
	sdp_mmc_init_controller();
	sdp_mmc_set_clk(SDP_MMC_MIN_CLOCK);

	if(sdp_mmc_go_idle_state() != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD0_ERR;
	}
	if(sdp_mmc_send_op_cond() != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD1_ERR;
	}

	if(sdp_mmc_all_send_cid() != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD2_ERR;
	}

	if(sdp_mmc_send_relative_addr() != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD3_ERR;
	}

	if(sdp_mmc_send_csd() != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD9_ERR;
	}	

	if(sdp_mmc_select_card(SEL) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD7_ERR;
	}

	if(sdp_mmc_send_ext_csd(pExt_csd) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD8_ERR;
	}
    (*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR) = 0;
    /* check if high capacity or not */
	if (*(pExt_csd+EXT_CSD_SEC_CNT + 0) || *(pExt_csd+EXT_CSD_SEC_CNT + 1) || *(pExt_csd+EXT_CSD_SEC_CNT + 2) || *(pExt_csd+EXT_CSD_SEC_CNT + 3)){
		ext_csd_sectors =
						*(pExt_csd+EXT_CSD_SEC_CNT + 0) << 0 |
						*(pExt_csd+EXT_CSD_SEC_CNT + 1) << 8 |
						*(pExt_csd+EXT_CSD_SEC_CNT + 2) << 16 |
						*(pExt_csd+EXT_CSD_SEC_CNT + 3) << 24;

		if(ext_csd_sectors > (2u * 1024 * 1024 * 1024) / 512)
			(*(volatile unsigned int*)MMC_HIGH_CAPA_TAG_ADDR)= 1;
	}
    
	if(sdp_mmc_switch(3, 183, 2, 0) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD6_ERR;
	}
	writel(SDP_MMC_CTYPE_8BIT_MODE, SDP_MMC_REG_CTYPE);

	if(sdp_mmc_switch(3, 185, 1, 0) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD6_ERR;
	}
	sdp_mmc_set_clk(SDP_MMC_PROCESSOR_CLK);

//	mmc_plat_loop(1000);

#ifdef SDP_MMC_DDRMODE
	if((pExt_csd[196] & SDP_MMC_DDR_52MHZ_12V) || (pExt_csd[196] & SDP_MMC_DDR_52MHZ_18_3V)){

		//High-speed Interface Timing
		if(sdp_mmc_switch(3, 185, 1, 0) != SDP_MMC_SUCCESS){
			return SDP_MMC_CMD6_ERR;
		}
		sdp_mmc_set_clk(SDP_MMC_PROCESSOR_CLK);

		//Bus Width Mode : 8 bit(DDR Mode)
		writel(SDP_MMC_CTYPE_8BIT_MODE, SDP_MMC_REG_CTYPE);
		if(sdp_mmc_switch(3, 183, 6, 0) != SDP_MMC_SUCCESS){
			return SDP_MMC_CMD6_ERR;
		}

		writel(SDP_MMC_UHS_DDR_REG(1), SDP_MMC_REG_UHS);
	}
#endif

	if(sdp_mmc_switch(3, 179, 0x49, 0) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD6_ERR;
	}

//	mmc_plat_loop(1000);

	if(sdp_mmc_switch(3, 177, 2, 0) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD6_ERR;
	}

//	mmc_plat_loop(1000);


	
	boot_size = (unsigned int)&__bss_start - (unsigned int)&_start; // uboot size

	boot_size = boot_size >> 9; //block count
	boot_size++; //prevent the cutting size about a decimal point of block size
	boot_size = boot_size << 9; // mmc's one block unit is 512bytes;

//	while(1){
//		err = 0;
//		writel(_end, SDP_MMC_REG_BYTCNT);
//	}
	/* copy from uboot program to ddr */
	err=sdp_mmc_RW(READ_MMC_MODE, BOOT_PARTITION_BOOT_ADDR, DDR_BOOT_ADDR, 128*1024);
	if(err != SDP_MMC_SUCCESS){
		return err;
	}

	if(sdp_mmc_switch(3, 179,0x48, 0) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD6_ERR;
	}

}

void __sdp_mmc(void){
	u32 err;
	err = sdp_mmc_init();
#ifdef SDP_MMC_DEBUG
	
	u32 err_value;

	if(err >= 0x100){
		while(1){
			err_value = err;
		}
	}
#endif

	

}
void sdp_mmc(void) __attribute__((weak, alias("__sdp_mmc")));

