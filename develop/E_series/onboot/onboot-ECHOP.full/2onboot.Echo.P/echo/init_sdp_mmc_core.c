#include <sp_setup.h>
#include <config.h>
#include <sdp_mmc.h>
#include <sdp_devmmc.h>

//u32 gERR=0x12345678;
//u32 gRESP[4];

//CardInfo_t gCard;


u32 mmc_desc_init(u32 desc_addr, u32 data_addr, u32 data_size){
	u32 i;
	u32 desc_num;
	sdp_mmc_desc *pdesc = NULL;
	sdp_mmc_desc *gpdesc;
	
	gpdesc = (sdp_mmc_desc *)desc_addr;
	desc_num = data_size >>  12;

	if((data_size - (SDP_MMC_DESC_MAX_DATA_SIZE * desc_num)) != 0){
		desc_num++;
	}
	if(data_size <=SDP_MMC_DESC_MAX_DATA_SIZE)
		desc_num = 1;

	for(i=0;i<(desc_num);i++){
		pdesc = &gpdesc[i];

		pdesc->desc0 = DescOwnByDma | DescSecAddrChained;
		pdesc->desc1 = SDP_MMC_DESC_MAX_DATA_SIZE;
		pdesc->desc2 = data_addr + (i * SDP_MMC_DESC_MAX_DATA_SIZE);
		pdesc->desc3 = (u32 *)(&gpdesc[ i + 1 ]);
	}
	
	pdesc = &gpdesc[0];
	pdesc->desc0 |= DescFirstDesc;
				
	pdesc = &gpdesc[i - 1];
	pdesc->desc0 |= DescLastDesc;
	pdesc->desc1 = (data_size - (SDP_MMC_DESC_MAX_DATA_SIZE * (desc_num-1)));
	pdesc->desc3 = 0x0;

	return 0;
}

void sdp_mmc_init_controller(void){
	u32 regval = 0;

#ifdef CONFIG_ECHO_S
	//Echo-S Pre Init Clock Delay setting
	*(volatile u32*)(0x30090924) |= (0x09<< 24); //sample
	*(volatile u32*)(0x30090928) |= (0x09<< 24);//driving
	*(volatile u32*)(0x30090954) |= (0x1<< 24);//s/w reset[mmc]	
#endif

#ifdef CONFIG_ECHO_B
	//Echo-S Pre Init Clock Delay setting[31:16] : reserved, [15:8] : driving, [7:0] : sampling
	*(volatile u32*)(0x300d0018) = (0x0509<< 0); //sample
	*(volatile u32*)(0x30090c48) |= (0x3<< 0); 
	*(volatile u32*)(0x30090c4c) |= (0x0<< 0); 
#endif

#ifdef CONFIG_ECHO_P
	//Echo-S Pre Init Clock Delay setting
	*(volatile u32*)(0x30090934) &= 0x0000ffff;
	*(volatile u32*)(0x30090934) |= (0x43C3<< 16); //sample
//	*(volatile u32*)(0x30090954) |= (0x1<< 24);//s/w reset[mmc]
	//after u-boot appears console line, EMAC uses SRAM 8KB , disable mmc sram using
//	*(volatile u32*)(0x30090cdc) |= (0x1<< 0);//s/w reset[mmc]
	
#endif

#ifdef CONFIG_FIRENZE
	//Firenze Pre Init Clock Delay setting
	*(volatile u32*)(0x30090CD0) |= (3 << 9);
#endif

	writel(SDP_MMC_BMOD_SWR, SDP_MMC_REG_BMOD); // It's automatically cleared after 1 clock cycle.
	do{
		regval = readl(SDP_MMC_REG_BMOD);
		regval &= SDP_MMC_BMOD_SWR;
		if(!regval) break;
	}while(1);

	writel(SDP_MMC_CTRL_CONTROLLER_RESET | SDP_MMC_CTRL_FIFO_RESET | SDP_MMC_CTRL_DMA_RESET, SDP_MMC_REG_CTRL);
	do{
		regval = readl(SDP_MMC_REG_CTRL);
		regval &= SDP_MMC_CTRL_CONTROLLER_RESET | SDP_MMC_CTRL_FIFO_RESET | SDP_MMC_CTRL_DMA_RESET;
		if(!regval) break;
	}while(1);

	

	/* Now make CTYPE to default i.e, all the cards connected will work in 1 bit mode initially*/
	writel(0x0, SDP_MMC_REG_CTYPE);

	/* Power up only those cards/devices which are connected 
		- Shut-down the card/device once wait for some time
		- Enable the power to the card/Device. wait for some time
	*/
	writel(0x0, SDP_MMC_REG_PWREN); 		// ?????
	writel(0x1, SDP_MMC_REG_PWREN);

	/* Set up the interrupt mask. 
		   - Clear the interrupts in any pending Wrinting 1's to RINTSTS
	   - Enable all the interrupts other than ACD in INTMSK register 
	   - Enable global interrupt in control register
	*/
	writel(0xFFFFFFFF, SDP_MMC_REG_RINTSTS);

	regval = SDP_MMC_INTMSK_ALL_ENABLED & ~SDP_MMC_INTMSK_ACD;
	writel(regval, SDP_MMC_REG_INTMSK);

	regval = SDP_MMC_CTRL_INT_ENABLE;
#ifdef SDP_MMC_USE_DMA
	regval |= SDP_MMC_CTRL_USE_INTERNAL_DMAC;
#endif
	writel(regval, SDP_MMC_REG_CTRL);

	/* Set Data and Response timeout to Maximum Value*/
	writel(0xFFFFFFFF, SDP_MMC_REG_TMOUT);

	writel(SDP_MMC_CLKENA_ALL_CCLK_ENABLE, SDP_MMC_REG_CLKENA);

	/* Set the card Debounce to allow the CDETECT fluctuations to settle down*/ 
	writel(DEFAULT_DEBNCE_VAL, SDP_MMC_REG_DEBNCE);

	/* Update the watermark levels to half the fifo depth
	   - while reset bitsp[27..16] contains FIFO Depth
	   - Setup Tx Watermark
	   - Setup Rx Watermark
	*/
	regval = SDP_MMC_FIFOTH_DW_DMA_MTS_8TRANS |
			 SDP_MMC_FIFOTH_RX_WMARK(7) |
			 SDP_MMC_FIFOTH_TX_WMARK(8);
	
	writel(regval, SDP_MMC_REG_FIFOTH);
#ifdef SDP_MMC_USE_DMA
		regval = SDP_MMC_BMOD_DE |
				 SDP_MMC_BMOD_FB |
				 SDP_MMC_BMOD_DSL(0);
		writel(regval, SDP_MMC_REG_BMOD);	
		writel(SDP_MMC_DESC_ADDR, SDP_MMC_REG_DBADDR);
		writel(0x337, SDP_MMC_REG_IDINTEN);
#endif

	writel(0x200, SDP_MMC_REG_BYTCNT);

}

void mmc_plat_loop(u32 counter){
	u32 i;
	
	for(i=0;i<FREQ_ARM*counter;i++)
		;
}

int mmc_poll_cmd_done(void){

	u32 num_of_retries = 0;
	while(1){
		if(num_of_retries > SDP_MMC_CMD_MAX_RETRIES){
//			gERR = ERR_MAX_RETRY;
			return SDP_MMC_ERROR;
		}
		if(((readl(SDP_MMC_REG_CMD)) & SDP_MMC_CMD_START_CMD) == 0){
			break;
		}
		num_of_retries++;
		mmc_plat_loop(10);
	}
	return SDP_MMC_SUCCESS;

}

void _mmc_wait_on_data_state_mc_busy(u32 timeout){
	while((readl(SDP_MMC_REG_STATUS) & 0x00000400) && timeout--);
}

void _mmc_wait_on_busy(u32 timeout){
	u32 delay = timeout * FREQ_ARM;
	while((readl(SDP_MMC_REG_STATUS) & SDP_MMC_STATUS_DAT_BUSY) && delay--);
}

void _mmc_wait_for_intr(u32 timeout){
	u32 delay = timeout * FREQ_ARM;
	while(!(readl(SDP_MMC_REG_RINTSTS) & SDP_MMC_INTMSK_CMD_DONE) && delay--);
}

void _mmc_wait_for_dto(u32 timeout){
	u32 delay = timeout * FREQ_ARM;
	while(!(readl(SDP_MMC_REG_RINTSTS) & SDP_MMC_INTMSK_DTO) && delay--);
}

int _mmc_get_err(void){
	volatile u32 u32rintsts=readl(SDP_MMC_REG_RINTSTS);
	u32 err=SDP_MMC_SUCCESS;
	
	if(u32rintsts & SDP_MMC_INTMSK_EBE){
		/*End-bit error (read)/write no CRC (EBE)*/
		err=SDP_MMC_ERROR;
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_SBE){
		/*Start-bit error (SBE)*/
		err=SDP_MMC_ERROR;		
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_HLE){
		/*Hardware locked write error (HLE)*/
		err=SDP_MMC_ERROR;		
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_FRUN){
		/*FIFO underrun/overrun error (FRUN)*/
		err=SDP_MMC_ERROR;		
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_HTO){
		/*Data starvation-by-host timeout (HTO)*/
		err=SDP_MMC_ERROR;		
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_DRTO){
		/*Data read timeout (DRTO)/Boot Data Start (BDS)*/
		err=SDP_MMC_ERROR;		
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_RTO){
		/*Response timeout (RTO)/Boot Ack Received (BAR)*/	
		err=SDP_MMC_ERROR;			
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_DCRC){
		/*Data CRC error (DCRC)*/
		err=SDP_MMC_ERROR;			
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_RCRC){
		/*Response CRC error (RCRC)*/
		err=SDP_MMC_ERROR;			
	}
	
	if(u32rintsts & SDP_MMC_INTMSK_RE){
		/*Response error (RE)*/
		err=SDP_MMC_ERROR;			
	}
	return err;

}

void mmc_response_status(void){
//		gRESP[0]=readl(SDP_MMC_REG_RESP0);
//		gRESP[1]=readl(SDP_MMC_REG_RESP1); 
//		gRESP[2]=readl(SDP_MMC_REG_RESP2); 
//		gRESP[3]=readl(SDP_MMC_REG_RESP3);
}

int mmc_send_cmd_wait(u32 u32cmd, u32 u32arg){
	u32 regval;
	u32 RESP[4];
	
	RESP[0] = 0;
	RESP[1] = 0;
	RESP[2] = 0;
	RESP[3] = 0;
	
	regval = readl(SDP_MMC_REG_CTRL);
	regval |= SDP_MMC_CTRL_FIFO_RESET | SDP_MMC_CTRL_DMA_RESET;
	writel(regval, SDP_MMC_REG_CTRL);

	do{
		regval = readl(SDP_MMC_REG_CTRL);
		regval &= (SDP_MMC_CTRL_FIFO_RESET | SDP_MMC_CTRL_DMA_RESET);
		if(!regval) break;
	}while(1);
	
	writel(SDP_MMC_RINTSTS_ALL_ENABLED, SDP_MMC_REG_RINTSTS);
	writel(0xffffffff, SDP_MMC_REG_IDSTS);

	writel(u32arg, SDP_MMC_REG_CMDARG);
	writel(u32cmd, SDP_MMC_REG_CMD);

	if(mmc_poll_cmd_done() != SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;
	}

	mmc_plat_loop(100);

	_mmc_wait_for_intr(20000);

	if(!(readl(SDP_MMC_REG_RINTSTS) & SDP_MMC_INTMSK_CMD_DONE)){
		return SDP_MMC_ERROR;
	}

	if(u32cmd & SDP_MMC_CMD_DATA_EXPECTED)
		_mmc_wait_for_dto(20000);

	_mmc_wait_on_busy(20000);
	
	mmc_response_status();

	return _mmc_get_err();
}

int sdp_mmc_set_clk(u32 clock){

	int retval = 0;
	u32 divider = 0;
	u32 clk_value;
	u32 clkena_val;

	if(clock >= SDP_MMC_MAX_CLK){
		if(SDP_MMC_PROCESSOR_CLK >= SDP_MMC_MAX_CLK)
			divider = 0x1;
		if(SDP_MMC_PROCESSOR_CLK < SDP_MMC_MAX_CLK)
			divider = 0x0;
	} else {
		clk_value = SDP_MMC_PROCESSOR_CLK;
		divider =1;
		while(1){
			if(clk_value < clock)
				break;

			clk_value = clk_value >> 1;	
			divider = divider << 1;
			if(divider == 0)
				divider = 1;
		}
		divider = divider >>1;
	}
	clkena_val = 0xffff;

// clock disable
	writel(0, SDP_MMC_REG_CLKENA);

	writel(0, SDP_MMC_REG_CMDARG);
	writel(SDP_MMC_UPDATE_CLOCK, SDP_MMC_REG_CMD);
	if(mmc_poll_cmd_done() != SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;
	}

// set divider value
	writel(0, SDP_MMC_REG_CLKDIV);		// ????
	writel(divider, SDP_MMC_REG_CLKDIV);	// ????
	writel(0, SDP_MMC_REG_CMDARG);
	writel(SDP_MMC_UPDATE_CLOCK, SDP_MMC_REG_CMD);
	if(mmc_poll_cmd_done() != SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;
	}

// clock enable
	writel(clkena_val, SDP_MMC_REG_CLKENA);
	writel(0, SDP_MMC_REG_CMDARG);
	writel(SDP_MMC_UPDATE_CLOCK, SDP_MMC_REG_CMD);
	if(mmc_poll_cmd_done() != SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;
	}

	return retval;
}

int sdp_mmc_go_idle_state(void){
	u32 u32cmd = CMD0 | SDP_MMC_CMD_START_CMD | SDP_MMC_CMD_SEND_INITIALIZATION;
	u32 u32arg = 0;

	if(SDP_MMC_SUCCESS != mmc_send_cmd_wait(u32cmd, u32arg)){
		return SDP_MMC_ERROR;
	}

	return SDP_MMC_SUCCESS;
}

int sdp_mmc_sd_send_if_cond(void){

	if(mmc_send_cmd_wait(CMD8 | SDP_MMC_CMD_START_CMD |SDP_MMC_CMD_RESPONSE_EXPECT,
		0x000001AA) != SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;
	}
	return SDP_MMC_SUCCESS;
			
}

int sdp_mmc_send_op_cond(void){
	u32 RESP[4];
	u32 u32cmd=0, u32arg=0;
	int i=0;
	u32 err = SDP_MMC_SUCCESS;
		
	u32cmd=CMD1|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_START_CMD;
	u32arg=0x40ff8000; 
		
	while(1)
	{
		err = mmc_send_cmd_wait(u32cmd, u32arg);
		RESP[0]=readl(SDP_MMC_REG_RESP0);
		if((err == SDP_MMC_SUCCESS) && (RESP[0] & MMC_CARD_BUSY)){
			break;
		}
	}

	return err;
}

int sdp_mmc_all_send_cid(void){
	u32 u32cmd=0, u32arg=0;

	u32cmd=CMD2|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_RESPONSE_LENGTH|SDP_MMC_CMD_CHECK_RESPONSE_CRC |
				SDP_MMC_CMD_START_CMD;
	u32arg=0;
		
	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;			
	}
	
	return SDP_MMC_SUCCESS;	
}

int sdp_mmc_send_relative_addr(void){
	u32 u32cmd=0, u32arg=0;
		
	u32cmd=CMD3|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_CHECK_RESPONSE_CRC |SDP_MMC_CMD_START_CMD ;
	
	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;			
	}
	
	return SDP_MMC_SUCCESS;

}

int sdp_mmc_send_csd(void){
	u32 u32cmd=0, u32arg=0;

	u32cmd=CMD9|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_RESPONSE_LENGTH|
				SDP_MMC_CMD_CHECK_RESPONSE_CRC | SDP_MMC_CMD_START_CMD;
	u32arg=0;
		
	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;			
	}
	
	return SDP_MMC_SUCCESS; 
}


u32 sdp_mmc_select_card(u32 select){
	u32 u32cmd=0, u32arg=0, err=SDP_MMC_SUCCESS;

	if(select == SEL){
		u32cmd=CMD7|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_CHECK_RESPONSE_CRC |SDP_MMC_CMD_START_CMD;
			
		if(SDP_MMC_SUCCESS!=mmc_send_cmd_wait(u32cmd,u32arg)){
			err=SDP_MMC_ERROR;
			goto out;
		}
	}
	else if(select == DESEL){
		/*Card can only be de-selected in prg/data/tran/stdby*/
		u32cmd=CMD7|SDP_MMC_CMD_START_CMD;			
		u32arg=0;
		
		if(SDP_MMC_SUCCESS!=mmc_send_cmd_wait(u32cmd,u32arg)){
			err=SDP_MMC_ERROR;
			goto out;
		}
	}
	else {
		err=SDP_MMC_ERROR;
	}
out:
	return err;

}

int sdp_mmc_send_ext_csd(u8 *ext_csd){
	u32 u32cmd;
	u32 u32arg;

	u32cmd = CMD8 | SDP_MMC_CMD_DATA_EXPECTED | SDP_MMC_CMD_CHECK_RESPONSE_CRC |SDP_MMC_CMD_WARVDATA_COMPLETE |
			SDP_MMC_CMD_RESPONSE_EXPECT | SDP_MMC_CMD_START_CMD ;
	u32arg = 0;

	writel(512, SDP_MMC_REG_BLKSIZ);
	writel(512, SDP_MMC_REG_BYTCNT);
	mmc_desc_init(SDP_MMC_DESC_ADDR, ext_csd, 512);
	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;			
	}
	
	return SDP_MMC_SUCCESS;
	

}


u32 sdp_mmc_switch_ext_csd(u8 access, u8 index, u8 value, u8 cmdset){
	u32 u32arg = 0;

	u32arg |= (access << 24);
	u32arg |= (index << 16);
	u32arg |= (value<< 8);
	u32arg |= (cmdset<< 0);
	return u32arg;
}

int sdp_mmc_switch(u8 access, u8 index, u8 value, u8 cmdset){
	u32 u32cmd=0, u32arg=0;
		
	u32cmd = CMD6|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_START_CMD|SDP_MMC_CMD_CHECK_RESPONSE_CRC ;
	
	u32arg = sdp_mmc_switch_ext_csd(access, index, value, cmdset);

	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		return SDP_MMC_ERROR;			
	}
	
	return SDP_MMC_SUCCESS;
}

int sdp_mmc_set_block_length(u32 u32blklen){
	u32 RESP[4];
	u32 u32cmd=0, u32arg=0, err=SDP_MMC_SUCCESS;
		
	u32cmd=CMD16|SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_CHECK_RESPONSE_CRC |SDP_MMC_CMD_START_CMD;
	u32arg=u32blklen;
		
	if(mmc_send_cmd_wait(u32cmd,u32arg)!=SDP_MMC_SUCCESS){
		err= SDP_MMC_ERROR;
		goto out;
	}
		
	/*Validate the card response*/
	RESP[0]=readl(SDP_MMC_REG_RESP0);
	if(RESP[0]&0xFDF98008){
		err= SDP_MMC_ERROR;
		goto out;		
	}
	
	/*This blklen will be used for all further cmds*/
//	gCard.u32blklen=u32blklen;
	
out:
	return err;
}

#ifndef SDP_MMC_USE_DMA
void sdp_mmc_do_pio(u32 direction, u8 *pui8buff, u32 u32size){
	u32* pdata=(u32*)pui8buff;
	u32 u32Local;
  /*Word alignment seems to be a problem here. Cant dereference pdata(typecasted from char*). Use local buffer*/
	  
  /*RX is nt empty OR Rx Data Full*/
	if(direction == READ_MMC_MODE){
		while(u32size>=4){
			if(readl(SDP_MMC_REG_STATUS) & SDP_MMC_STATUS_FIFO_EMPTY){
				continue;
			}
			*pdata++ = readl(SDP_MMC_REG_FIFODAT);
			pui8buff+=4;
			u32size-=4;
		}
		if(u32size){
			/*Partial data*/
			u32Local = readl(SDP_MMC_REG_FIFODAT);
			memcpy(pui8buff,&u32Local,u32size);
			pui8buff+=u32size;			
		}
	}else if(direction == WRITE_MMC_MODE){
		while(u32size>=4){
			if(readl(SDP_MMC_REG_STATUS) & SDP_MMC_STATUS_FIFO_FULL){
				continue;
			}
			writel(*pdata, SDP_MMC_REG_FIFODAT);
			pdata++;
			pui8buff+=4;
			u32size-=4;
		}
		if(u32size){
			/*Partial data*/
			memcpy(&u32Local,pui8buff,u32size);
			writel(u32Local, SDP_MMC_REG_FIFODAT);
			memcpy(pui8buff,&u32Local,u32size);
			pui8buff+=u32size;			
		}	
	}
}
#endif
int sdp_mmc_read_write(u32 direction, u32 u32addr, u8 *pui8buff, u32 u32size_in_bytes){
	u32 RESP[4];
	u32 u32cmd=0, u32arg=0;
	u32 err=SDP_MMC_SUCCESS;
	u32 u32blklen;
		
	/* Reset internal FIFO */
	writel(readl(SDP_MMC_REG_CTRL) | SDP_MMC_CTRL_FIFO_RESET | SDP_MMC_CTRL_DMA_RESET,  SDP_MMC_REG_CTRL);
	
	u32blklen=MMC_BLK_SIZE;
	
	/*Set MMCIF_REG_BLKSIZ*/
	writel(u32blklen, SDP_MMC_REG_BLKSIZ);
		
	/*Set MMCIF_REG_BYTCNT*/
	writel(u32size_in_bytes, SDP_MMC_REG_BYTCNT);
	
		
	u32cmd=SDP_MMC_CMD_START_CMD|SDP_MMC_CMD_DATA_EXPECTED|
			SDP_MMC_CMD_RESPONSE_EXPECT|SDP_MMC_CMD_WARVDATA_COMPLETE;
		
	if(direction == READ_MMC_MODE){
		/*Read*/
		u32cmd|=(u32size_in_bytes>u32blklen)?CMD18:CMD17;		
	}else if(direction == WRITE_MMC_MODE){
		/*Write*/
		u32cmd|=(u32size_in_bytes>u32blklen)?CMD25:CMD24;
		u32cmd|=SDP_MMC_CMD_READ_WRITE;	
	}
		
#ifdef SDP_MMC_AUTO_STOP
	if(u32size_in_bytes>u32blklen){
		u32cmd|=SDP_MMC_CMD_SEND_AUTO_STOP;
	}
#endif
	
	/*Pass Read/Write addr as arg*/
	u32arg=u32addr;
	
	if(SDP_MMC_SUCCESS!=mmc_send_cmd_wait(u32cmd,u32arg)){
		err=SDP_MMC_ERROR;
		return err;
	}
	
	/*Validate the card response*/
	RESP[0]=readl(SDP_MMC_REG_RESP0);
	if(RESP[0]&0xFDF98008){
		err=SDP_MMC_ERROR;
		return err;
	}
#ifndef SDP_MMC_USE_DMA

	/*Do PIO Tx/Rx*/
	sdp_mmc_do_pio(direction, pui8buff, u32size_in_bytes);
	
#endif		
	/*Wait for DTO INTR*/
	_mmc_wait_for_dto(0x1000);

	if(u32size_in_bytes == u32blklen){
		/*All data transferred. Send STOP transmission */
		u32cmd=CMD12|SDP_MMC_CMD_START_CMD|SDP_MMC_CMD_RESPONSE_EXPECT|
				SDP_MMC_CMD_STOP_ABORT_CMD|SDP_MMC_CMD_WARVDATA_COMPLETE;
		u32arg=0;
		
		if(SDP_MMC_SUCCESS!=mmc_send_cmd_wait(u32cmd,u32arg)){
			err=SDP_MMC_ERROR;
			return err;
		}		
	}
	return err;
}

int sdp_mmc_RW(u32 bread, u32 card_addr, u32 ram_addr, u32 u32size){

	u8 * pui8buff = (u8 *)ram_addr;
#ifndef SDP_MMC_DDRMODE	
	if(sdp_mmc_set_block_length(512) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD16_ERR;
	}
#endif

	mmc_desc_init(SDP_MMC_DESC_ADDR, ram_addr, u32size);

	if(sdp_mmc_read_write(bread, card_addr, pui8buff, u32size) != SDP_MMC_SUCCESS){
		return SDP_MMC_CMD18_ERR;
	}
	
	return SDP_MMC_SUCCESS;
}

