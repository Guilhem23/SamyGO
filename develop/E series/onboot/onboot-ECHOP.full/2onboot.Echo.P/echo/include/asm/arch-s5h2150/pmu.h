/*
 * linux/include/asm-arm/arch-s5h2150/pmu.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: ij.jang@samsung.com
 *
 */

#ifndef _S5H2150_PMU_H_
#define _S5H2150_PMU_H_

/* clock & power management */
#define V_PMU(offset)    (*(volatile unsigned *)(V_PMU_BASE+(offset)))

#define O_PLLCON	(0x00)
#define O_MODCON	(0x04)
#define O_FCLKCON	(0x08)
#define O_HCLKCON	(0x0C)
#define O_PCLKCON	(0x10)
#define O_CLKMOD	(0x14)
#define O_LDTCNT	(0x18)
#define O_RSETCON	(0x1C)
/* reserved : manual wrong, this is right */
#define O_RESPTIM	(0x24)
#define O_USB_PCI_SET	(0x28)
#define O_SOFTRES	(0x2C)
#define O_ECLKCON	(0x30)
#define O_BUSPLLCON	(0x34)

/* for C */
#define R_PLLCON	V_PMU(O_PLLCON)
#define R_MODCON	V_PMU(O_MODCON)
#define R_FCLKCON	V_PMU(O_FCLKCON)
#define R_HCLKCON	V_PMU(O_HCLKCON)
#define R_PCLKCON	V_PMU(O_PCLKCON)
#define R_CLKMOD	V_PMU(O_CLKMOD)
#define R_LDTCNT	V_PMU(O_LDTCNT)
#define R_RSETCON	V_PMU(O_RSETCON)
/* reserved */
#define R_RESPTIM	V_PMU(O_RESPTIM)
#define R_USB_PCI_SET	V_PMU(O_USB_PCI_SET)
#define R_SOFTRES	V_PMU(O_SOFTRES)
#define R_ECLKCON	V_PMU(O_ECLKCON)
#define R_BUSPLLCON	V_PMU(O_BUSPLLCON)

#endif
