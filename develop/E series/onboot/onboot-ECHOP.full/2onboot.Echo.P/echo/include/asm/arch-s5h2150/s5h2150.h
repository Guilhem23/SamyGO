/*
 * linux/include/asm-arm/arch-s5h2150/s5h2150.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: ij.jang@samsung.com
 *
 */

#ifndef __S5H2150_H
#define __S5H2150_H

/*
 * S5H2150 internal registers
 *
 * Be careful !!!!
 * PAGE_OFFSET + VMALLOC_END = 0xD0000000
 *
 */

#include <asm/arch/smc.h>
#include <asm/arch/ddrc.h>
#include <asm/arch/timer.h>
#include <asm/arch/wdt.h>
#include <asm/arch/pmu.h>
#include <asm/arch/uart.h>
#include <asm/arch/gpio.h>
#include <asm/arch/interrupt.h>
#include <asm/arch/pci.h>

/* macros for C */
#define REG_PUT(ADDR, VAL)	ADDR = VAL
#define REG_GET(ADDR)		(ADDR)

/* BASE addresses */
#define P_SFR_BASE0		(0x30010000)
#define V_SFR_BASE0		(0x30010000)

#define	P_SMC_BASE		(0x30020000)
#define	V_SMC_BASE		((P_SMC_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define	P_PCI_BASE		(0x30030000)
#define	V_PCI_BASE		((P_PCI_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_TIMER_BASE		(0x30090400)
#define	V_TIMER_BASE		((P_TIMER_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_WDT_BASE		(0x30090600)
#define	V_WDT_BASE		((P_WDT_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define	P_PMU_BASE		(0x30090800)
#define	V_PMU_BASE		((P_PMU_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_UART_BASE		(0x30090A00)
#define	V_UART_BASE		((P_UART_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_GPIO_BASE		(0x30090C00)
#define	V_GPIO_BASE		((P_GPIO_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_INT_BASE		(0x30090F00)
#define	V_INT_BASE		((P_INT_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_DDRC_BASE		(0x30404C00)
#define	V_DDRC_BASE		((P_DDRC_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define P_OHCI0_BASE		(0x30088000)
#define V_OHCI0_BASE		((P_OHCI0_BASE - P_SFR_BASE0) + V_SFR_BASE0)

#define REQ_FCLK	1
#define REQ_DCLK	2
#define REQ_HCLK	3
#define REQ_PCLK	4

#define MHZ		1000000

#define rTSD_STC_BASE		(0x30110080)
#endif
