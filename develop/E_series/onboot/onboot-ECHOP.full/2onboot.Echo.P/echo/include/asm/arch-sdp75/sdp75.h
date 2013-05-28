/*
 * linux/include/asm-arm/arch-sdp/sdp75.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef __SDP75_H
#define __SDP75_H

#if 0
#include "bitfield.h"
#else
/* macros for C */
#define	REG_PTR(ADDR)			((volatile unsigned	int*)ADDR)
#define	REG_VAL(ADDR) 			(*((volatile unsigned int*)ADDR))
#define REG_PUT(ADDR, DATA)		ADDR = DATA
#define REG_GET(ADDR)			ADDR
#endif

/*
 * SDP75 internal registers
 *
 * Be careful !!!!
 * PAGE_OFFSET + VMALLOC_END = 0xD0000000
 *
 */
#define V_SFR_BASE0          	(0x30010000)
#define P_SFR_BASE0          	(0x30010000)

#define V_SFR_BASE1          	(0xB0400000)
#define P_SFR_BASE1          	(0xB0400000)

#define P_SMC_BASE           	(0x30020000)
#define V_SMC_BASE           	((P_SMC_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_XMIF_W_BASE		(0x30400000)
#define V_XMIF_W_BASE	  	((P_XMIF_W_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_XMIF_E_BASE	     	(0xB0400000)
#define V_XMIF_E_BASE		((P_XMIF_E_BASE - P_SFR_BASE1) + V_SFR_BASE1)

#define P_TIMER_BASE         	(0x30090400)
#define V_TIMER_BASE         	((P_TIMER_BASE - P_SFR_BASE0) + V_SFR_BASE0)
/*
#define P_RTC_BASE           (0x30090500)
#define V_RTC_BASE           (V_SFR_BASE0 - P_SFR_BASE0 + P_RTC_BASE)
*/
#define P_WDT_BASE           	(0x30090600)
#define V_WDT_BASE           	((P_WDT_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_PMU_BASE           	(0x30090800)
#define V_PMU_BASE           	((P_PMU_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_UART_BASE          	(0x30090A00)
#define V_UART_BASE          	((P_UART_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_GPIO_BASE          	(0x30090C00)
#define V_GPIO_BASE          	((P_GPIO_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_INT_BASE           	(0x30090F00)
#define V_INT_BASE           	((P_INT_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_PCI_BASE           	(0x30030000)
#define V_PCI_BASE           	((P_PCI_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#include <asm/arch/smc.h>
#include <asm/arch/xmif.h>
#include <asm/arch/timer.h>
#include <asm/arch/wdt.h>
#include <asm/arch/pmu.h>
#include <asm/arch/uart.h>
#include <asm/arch/gpio.h>
#include <asm/arch/interrupt.h>
#include <asm/arch/pci.h>

#if 0
/* Linux clock api definition */ 
// PMUCON define
#define GET_MDIV(val)	((val >> 8) & 0xFF)
#define GET_PDIV(val)	((val >> 2) & 0x3F)

// HCLK Clock define
#define CLK_SMC		2
#define CLK_USB		3

// PCKL Clock define 
#define CLK_I2C		0
#define CLK_SSP		1
#define	CLK_IRR		2
#define CLK_TIMER	3
#define CLK_RTC		4
#define CLK_WDT		5
#define CLK_IDLE	6
#define CLK_UART	7
#define CLK_INTC	8
#define CLK_GPIO	9
#define CLK_AIOCKG	10

// CLKMOD Define
#define GET_FCLK_MOD(val)	((val >> 8) & 0x7)
#define GET_HCLK_MOD(val)	((val >> 4) & 0xF)
#define GET_PCLK_MOD(val)	(val & 0xF)

#endif

#define REQ_FCLK	1
#define REQ_DCLK	2
#define REQ_HCLK	3
#define REQ_PCLK	4

#define MHZ		1000000

#endif /* __SDP75_H */
