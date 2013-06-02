/*
 * u-boot-1.1.3	port for S5H2111
 *
 * D-TV	development	team, SYS.LSI
 * Copyright(c)	2005 Samsung Electronics, Inc.
 * All rights reserved.
 *
 */

/**
 * s5h2111.h : define registers.
 *
 * @author	 : Ikjoon Jang (ij.jang@samsung.com)
 * @see		 : S5H2110 User's manual
 * @version	 : $Revision$
 *
 * modification	history
 * -----------------------------
 * 25, Jan, 2006 : ij.jang copy from aspen
 * 10, Nov,	2005 : ij.jang created.
 */

#ifndef	_S5H2111_H_
#define	_S5H2111_H_

/* macros */
#define	REG(BLK, N)				(BLK##_##BASE +	o##BLK##_##N)
#define	REG_BASE(BLK, B, N)		(B##_##BASE	+ o##BLK##_##N)

/* macros for C */
#define	REG_PTR(ADDR)			((volatile unsigned	int*)ADDR)
#define	REG_VAL(ADDR) 			(*((volatile unsigned int*)ADDR))
#define REG_PUT(ADDR, DATA)		REG_VAL(ADDR) = DATA
#define REG_GET(ADDR)			REG_VAL(ADDR)

/* Naming convention of	definitions
	o<BLOCK>_<REGNAME> : offset	from a block's base	address
		ex)	oPLL_PLLCON	: PLLCON register of PLL block

	<BLOCK>_BASE : base	address	of a block
		ex)	PLL_BASE : base	address	of PLL register	block

	r<REGNAME> : direct	address	of register
		ex)	rPLLCON	: address of PLLCON	register
*/

/********************************************************************
 *	Power management & PLL (PLL)
 ********************************************************************/
#define	PLL_BASE			0x30090800

#define	oPLL_PLLCON			0x00
#define	oPLL_MODCON			0x04
#define	oPLL_HCLKCON		0x0C
#define	oPLL_PCLKCON		0x10
#define	oPLL_CLKMOD			0x14
#define	oPLL_LDTCNT			0x18
#define	oPLL_RSETCON		0x1C
#define	oPLL_RESPTIM		0x24
#define	oPLL_SOFTRES		0x2C
#define	oPLL_ECLKCON		0x30

#define	rPLLCON				REG(PLL, PLLCON)
#define	rMODCON				REG(PLL, MODCON)
#define	rHCLKCON			REG(PLL, HCLKCON)
#define	rPCLKCON			REG(PLL, PCLKCON)
#define	rCLKMOD				REG(PLL, CLKMOD)
#define	rLDTCNT				REG(PLL, LDTCNT)
#define	rRSETCON			REG(PLL, RSETCON)
#define	rRESPTIM			REG(PLL, RESPTIM)
#define	rSOFTRES			REG(PLL, SOFTRES)
#define	rECLKCON			REG(PLL, ECLKCON)

/********************************************************************
 *	Static Memory Controller (SMC)
 ********************************************************************/
#define	SMC_BASE			0x30020000

/* bank 0 */
#define	oSMC_SMBIDCYR2		0x0038	/* Idle	Cycle control register */
#define	oSMC_SMBWST1R2		0x003C	/* Wait	state 1	control	register */
#define	oSMC_SMBWST2R2		0x0040	/* Wait	state 2	control	register */
#define	oSMC_SMBWSTOENR2	0x0044	/* OE assertion delay control register */
#define	oSMC_SMBWSTWENR2	0x0048	/* WE assertion delay control register */
#define	oSMC_SMBCR2			0x004C	/* Control register */
#define	oSMC_SMBSR2			0x0050	/* Status register */
/* bank 1 */
#define	oSMC_SMBIDCYR1		(0x001C)
#define	oSMC_SMBWST1R1		(0x0020)
#define	oSMC_SMBWST2R1		(0x0024)
#define	oSMC_SMBWSTOENR1	(0x0028)
#define	oSMC_SMBWSTWENR1	(0x002C)
#define	oSMC_SMBCR1		(0x0030)
#define	oSMC_SMBSR1		(0x0034)
/* bank 2 */
#define	oSMC_SMBIDCYR0		(0x0000)
#define	oSMC_SMBWST1R0		(0x0004)
#define	oSMC_SMBWST2R0		(0x0008)
#define	oSMC_SMBWSTOENR0	(0x000C)
#define	oSMC_SMBWSTWENR0	(0x0010)
#define	oSMC_SMBCR0		(0x0014)
#define	oSMC_SMBSR0		(0x0018)
/* bank 3 ~ 6 : not defined */
/* bank 7 */
#define	oSMC_SMBIDCYR7		0x00C4	/* Idle	Cycle control register */
#define	oSMC_SMBWST1R7		0x00C8	/* Wait	state 1	control	register */
#define	oSMC_SMBWST2R7		0x00CC	/* Wait	state 2	control	register */
#define	oSMC_SMBWSTOENR7	0x00D0	/* OE assertion delay control register */
#define	oSMC_SMBWSTWENR7	0x00D4	/* WE assertion delay control register */
#define	oSMC_SMBCR7		0x00D8	/* Control register */
#define	oSMC_SMBSR7		0x00DC	/* Status register */

#define	oSMC_SMBEWS		0x00E0	/* External Wait Status */

#define	oSMC_SMCPeriphID0	0x0FE0	/* Peripheral ID register bits [7:0] */
#define	oSMC_SMCPeriphID1	0x0FE4	/* Peripheral ID register bits [15:8] */
#define	oSMC_SMCPeriphID2	0x0FE8	/* Peripheral ID register bits [23:16] */
#define	oSMC_SMCPeriphID3	0x0FEC	/* Peripheral ID register bits [31:24] */
#define	oSMC_SMCPCellID0	0x0FF0	/* Prime Cell ID register bits [7:0] */
#define	oSMC_SMCPCellID1	0x0FF4	/* Prime Cell ID register bits [15:8] */
#define	oSMC_SMCPCellID2	0x0FF8	/* Prime Cell ID register bits [23:16] */
#define	oSMC_SMCPCellID3	0x0FFC	/* Prime Cell ID register bits [31:24] */

#define oSMC_SMCCLKSTOP		0x01E8	/* OneNAND Clock */
#define oSMC_SMCSYNCEN		0x01EC	/* OneNAND Clock enable */

#define	rSMBIDCYR2			REG(SMC, SMBIDCYR2)
#define	rSMBWST1R2			REG(SMC, SMBWST1R2)
#define	rSMBWST2R2			REG(SMC, SMBWST2R2)
#define	rSMBWSTOENR2		REG(SMC, SMBWSTOENR2)
#define	rSMBWSTWENR2		REG(SMC, SMBWSTWENR2)
#define	rSMBCR2				REG(SMC, SMBCR2)
#define	rSMBSR2				REG(SMC, SMBSR2)

#define	rSMBIDCYR1			REG(SMC, SMBIDCYR1)
#define	rSMBWST1R1			REG(SMC, SMBWST1R1)
#define	rSMBWST2R1			REG(SMC, SMBWST2R1)
#define	rSMBWSTOENR1		REG(SMC, SMBWSTOENR1)
#define	rSMBWSTWENR1		REG(SMC, SMBWSTWENR1)
#define	rSMBCR1				REG(SMC, SMBCR1)
#define	rSMBSR1				REG(SMC, SMBSR1)

#define	rSMBIDCYR0			REG(SMC, SMBIDCYR0)
#define	rSMBWST1R0			REG(SMC, SMBWST1R0)
#define	rSMBWST2R0			REG(SMC, SMBWST2R0)
#define	rSMBWSTOENR0		REG(SMC, SMBWSTOENR0)
#define	rSMBWSTWENR0		REG(SMC, SMBWSTWENR0)
#define	rSMBCR0				REG(SMC, SMBCR0)
#define	rSMBSR0				REG(SMC, SMBSR0)

#define	rSMBIDCYR7			REG(SMC, SMBIDCYR7)
#define	rSMBWST1R7			REG(SMC, SMBWST1R7)
#define	rSMBWST2R7			REG(SMC, SMBWST2R7)
#define	rSMBWSTOENR7		REG(SMC, SMBWSTOENR7)
#define	rSMBWSTWENR7		REG(SMC, SMBWSTWENR7)
#define	rSMBCR7				REG(SMC, SMBCR7)
#define	rSMBSR7				REG(SMC, SMBSR7)

#define	rSMBEWS				REG(SMC, SMBEWS)

#define	rSMCPeriphID0		REG(SMC, SMCPeriphID0)
#define	rSMCPeriphID1		REG(SMC, SMCPeriphID1)
#define	rSMCPeriphID2		REG(SMC, SMCPeriphID2)
#define	rSMCPeriphID3		REG(SMC, SMCPeriphID3)
#define	rSMCPCellID0		REG(SMC, SMCPCellID0)
#define	rSMCPCellID1		REG(SMC, SMCPCellID1)
#define	rSMCPCellID2		REG(SMC, SMCPCellID2)
#define	rSMCPCellID3		REG(SMC, SMCPCellID3)

#define rSMCCLKSTOP		REG(SMC, SMCCLKSTOP)
#define rSMCSYNCEN		REG(SMC, SMCSYNCEN)

/********************************************************************
 * DDR Controller
 ********************************************************************/
#define	DDRC_BASE			0x30160100
#define  DDRC_ADC			0x30160158

#define	oDDRC_PRBR			0x00	/* Physical	row	boundary R */
#define	oDDRC_TR			0x04	/* Refresh control R */
#define	oDDRC_MMU_CON		0x08	/* Memory controller control R */
#define	oDDRC_DCR1			0x0C	/* Delay control R 1 */
#define	oDDRC_DCR3			0x10	/* Delay control R 3 */
#define	oDDRC_DCR4			0x14	/* Delay control R 4 */
#define	oDDRC_DCR5			0x18	/* Delay control R 5 */
#define	oDDRC_DCR6			0x1C	/* Delay control R 6 */
#define	oDDRC_DCR7			0x20	/* Delay control R 7 */
#define	oDDRC_DCR8			0x24	/* Delay control R 8 */
#define	oDDRC_PCR			0x28	/* Processor command R */
#define	oDDRC_MCSR			0x30	/* Memory controller status	R */
#define	oDDRC_MCBSR			0x34	/* Memory controller buffer	status R*/
#define	oDDRC_SELPATH		0x40	/* Response	path selector */
#define	oDDRC_SSTLPADC		0x48	/* SSTL	pad	control	R */

#define	rPRBR				REG(DDRC, PRBR)
#define	rTR					REG(DDRC, TR)
#define	rMMU_CON			REG(DDRC, MMU_CON)
#define	rDCR1				REG(DDRC, DCR1)
#define	rDCR3				REG(DDRC, DCR3)
#define	rDCR4				REG(DDRC, DCR4)
#define	rDCR5				REG(DDRC, DCR5)
#define	rDCR6				REG(DDRC, DCR6)
#define	rDCR7				REG(DDRC, DCR7)
#define	rDCR8				REG(DDRC, DCR8)
#define	rPCR				REG(DDRC, PCR)
#define	rMCSR				REG(DDRC, MCSR)
#define	rMCBSR				REG(DDRC, MCBSR)
#define	rSELPATH			REG(DDRC, SELPATH)
#define	rSSTLPADC			REG(DDRC, SSTLPADC)

/********************************************************************
 *	GPIO Ports (GPIO)
 ********************************************************************/
#define GPIO_BASE			0x30090C00

#define oGPIO_GPIO_SEL0		0x00
#define oGPIO_EXT_INT_MASK	0x04
#define oGPIO_GPIO0_DAT		0x08
#define oGPIO_GPIO0_CON		0x0C
#define oGPIO_GPIO1_DAT		0x10
#define oGPIO_GPIO1_CON		0x14
#define oGPIO_GPIO2_DAT		0x18
#define oGPIO_GPIO2_CON		0x1C
#define oGPIO_GPIO3_DAT		0x20
#define oGPIO_GPIO3_CON		0x24
#define oGPIO_GPIO4_DAT		0x28
#define oGPIO_GPIO4_CON		0x2C
#define oGPIO_GPIO5_DAT		0x30
#define oGPIO_GPIO5_CON		0x34
#define oGPIO_GPIO6_DAT		0x38
#define oGPIO_GPIO6_CON		0x3C
#define oGPIO_GPIO7_DAT		0x40
#define oGPIO_GPIO7_CON		0x44
#define oGPIO_GPIO8_DAT		0x48
#define oGPIO_GPIO8_CON		0x4C
#define oGPIO_GPIO9_DAT		0x50
#define oGPIO_GPIO9_CON		0x54
#define oGPIO_EXT_INT_CON	0x58
#define oGPIO_PIN_SEL_CON	0x5C

#define rGPIO_SEL0			REG(GPIO, GPIO_SEL0)
#define rEXT_INT_MASK	    REG(GPIO, EXT_INT_MASK)
#define rGPIO0_DAT		    REG(GPIO, GPIO0_DAT)
#define rGPIO0_CON		    REG(GPIO, GPIO0_CON)
#define rGPIO1_DAT		    REG(GPIO, GPIO1_DAT)
#define rGPIO1_CON		    REG(GPIO, GPIO1_CON)
#define rGPIO2_DAT		    REG(GPIO, GPIO2_DAT)
#define rGPIO2_CON		    REG(GPIO, GPIO2_CON)
#define rGPIO3_DAT		    REG(GPIO, GPIO3_DAT)
#define rGPIO3_CON		    REG(GPIO, GPIO3_CON)
#define rGPIO4_DAT		    REG(GPIO, GPIO4_DAT)
#define rGPIO4_CON		    REG(GPIO, GPIO4_CON)
#define rGPIO5_DAT		    REG(GPIO, GPIO5_DAT)
#define rGPIO5_CON		    REG(GPIO, GPIO5_CON)
#define rGPIO6_DAT		    REG(GPIO, GPIO6_DAT)
#define rGPIO6_CON		    REG(GPIO, GPIO6_CON)
#define rGPIO7_DAT		    REG(GPIO, GPIO7_DAT)
#define rGPIO7_CON		    REG(GPIO, GPIO7_CON)
#define rGPIO8_DAT		    REG(GPIO, GPIO8_DAT)
#define rGPIO8_CON		    REG(GPIO, GPIO8_CON)
#define rGPIO9_DAT		    REG(GPIO, GPIO9_DAT)
#define rGPIO9_CON		    REG(GPIO, GPIO9_CON)
#define rEXT_INT_CON	    REG(GPIO, EXT_INT_CON)
#define rPIN_SEL_CON	    REG(GPIO, PIN_SEL_CON)

/********************************************************************
 *	16-Bit timers
 ********************************************************************/
#define TIMER_BASE			0x30090400

#define oTIMER_TMDMASEL		0x0C
#define oTIMER_TMCON0		0x00
#define oTIMER_TMDATA0		0x04
#define oTIMER_TMCNT0		0x08
#define oTIMER_TMCON1		0x10
#define oTIMER_TMDATA1		0x14
#define oTIMER_TMCNT1		0x18

#define	rTMDMASEL			REG(TIMER, TMDMASEL)
#define	rTMCON0				REG(TIMER, TMCON0)
#define	rTMDATA0			REG(TIMER, TMDATA0)	
#define	rTMCNT0				REG(TIMER, TMCNT0)	
#define	rTMCON1				REG(TIMER, TMCON1)
#define	rTMDATA1			REG(TIMER, TMDATA1)	
#define	rTMCNT1				REG(TIMER, TMCNT1)	

/********************************************************************
 *	UART Controller(UART)
 ********************************************************************/
#define	UART_BASE			0x30090A00

#define	oUART_ULCON0		0x00	/* UART	channel	0 line control register	*/
#define	oUART_ULCON1		0x40	/* UART	channel	1 line control register	*/
#define	oUART_UCON0			0x04	/* UART	channel	0 control register */
#define	oUART_UCON1			0x44	/* UART	channel	1 control register */
#define	oUART_UFCON0		0x08	/* UART	channel	0 FIFO control register	*/
#define	oUART_UFCON1		0x48	/* UART	channel	1 FIFO control register	*/
#define	oUART_UMCON0		0x0C	/* UART	channel	0 Modem	control	register */
#define	oUART_UTRSTAT0		0x10	/* UART	channel	0 Tx/Rx	status register	*/
#define	oUART_UTRSTAT1		0x50	/* UART	channel	1 Tx/Rx	status register	*/
#define	oUART_UERSTAT0		0x14	/* UART	channel	0 Rx error status register */
#define	oUART_UERSTAT1		0x54	/* UART	channel	1 Rx error status register */
#define	oUART_UFSTAT0		0x18	/* UART	channel	0 FIFO status register */
#define	oUART_UFSTAT1		0x58	/* UART	channel	1 FIFO status register */
#define	oUART_UMSTAT0		0x1C	/* UART	channel	0 Modem	status register	*/
#define	oUART_UTXH0			0x20	/* UART	channel	0 transmit buffer register */
#define	oUART_UTXH1			0x60	/* UART	channel	1 transmit buffer register */
#define	oUART_URXH0			0x24	/* UART	channel	0 receive buffer register */
#define	oUART_URXH1			0x64	/* UART	channel	1 receive buffer register */
#define	oUART_UBRDIV0		0x28	/* Baud	rate divisor register 0	*/
#define	oUART_UBRDIV1		0x68	/* Baud	rate divisor register 1	*/

#define	rULCON0				REG(UART, ULCON0)
#define	rULCON1				REG(UART, ULCON1)
#define	rUCON0				REG(UART, UCON0)
#define	rUCON1				REG(UART, UCON1)
#define	rUFCON0				REG(UART, UFCON0)
#define	rUFCON1				REG(UART, UFCON1)
#define	rUMCON0				REG(UART, UMCON0)
#define	rUTRSTAT0			REG(UART, UTRSTAT0)
#define	rUTRSTAT1			REG(UART, UTRSTAT1)
#define	rUERSTAT0			REG(UART, UERSTAT0)
#define	rUERSTAT1			REG(UART, UERSTAT1)
#define	rUFSTAT0			REG(UART, UFSTAT0)
#define	rUFSTAT1			REG(UART, UFSTAT1)
#define	rUMSTAT0			REG(UART, UMSTAT0)
#define	rUTXH0				REG(UART, UTXH0)
#define	rUTXH1				REG(UART, UTXH1)
#define	rURXH0				REG(UART, URXH0)
#define	rURXH1				REG(UART, URXH1)
#define	rUBRDIV0			REG(UART, UBRDIV0)
#define	rUBRDIV1			REG(UART, UBRDIV1)

/********************************************************************
 *	Interrupt Controller(INC)
 ********************************************************************/
#define	INTC_BASE			0x30090F00

/* not all registers are defined. */
#define	oINTC_INTCON		0x00
#define	oINTC_INTPND		0x04
#define	oINTC_INTMOD		0x08
#define	oINTC_INTMSK		0x0C

#define	rINTCON				REG(INTC, INTCON)
#define	rINTPND				REG(INTC, INTPND)
#define	rINTMOD				REG(INTC, INTMOD)
#define	rINTMSK				REG(INTC, INTMSK)

/********************************************************************
 *	Watch-dog timer(WATCHDOG)
 ********************************************************************/
#define	WATCHDOG_BASE		0x30090600

#define	oWATCHDOG_WTPSCLR	0x00
#define	oWATCHDOG_WTCON		0x04
#define	oWATCHDOG_WTCNT		0x08

#define	rWTPSCLR			REG(WATCHDOG, WTPSCLR)
#define	rWTCON				REG(WATCHDOG, WTCON)
#define	rWTCNT				REG(WATCHDOG, WTCNT)

/********************************************************************
 *	TSD 
 ********************************************************************/
#define rTSD_STC_BASE		(0x30190100)

#endif	/* _S5H2111_H_ */

