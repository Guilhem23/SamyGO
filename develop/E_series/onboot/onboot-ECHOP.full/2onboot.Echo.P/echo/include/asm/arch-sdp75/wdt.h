/*
 * linux/include/asm-arm/arch-sdp75/wdt.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_WDT_H_
#define _SDP75_WDT_H_

/* watch dog timer */
#define V_WDT(offset)    (*(volatile unsigned *)(V_WDT_BASE+(offset)))

#define O_WTPSV			0x00	
#define O_WTCON	                0x04
#define O_WTCNT	                0x08

#define R_WTPSV			V_WDT(O_WTPSV) /* Watch-dog timer prescaler value */
#define R_WTCON			V_WDT(O_WTCON) /* Watch-dog timer mode */
#define R_WTCNT			V_WDT(O_WTCNT) /* Eatch-dog timer count */

#endif //_SDP75_WDT_H_


