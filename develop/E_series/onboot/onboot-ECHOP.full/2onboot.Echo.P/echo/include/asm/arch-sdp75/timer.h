/*
 * linux/include/asm-arm/arch-sdp75/timer.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_TIMER_H_
#define _SDP75_TIMER_H_

/* timer */
#define V_TIMER(t, offset)	\
    (*(volatile unsigned *)(V_TIMER_BASE+(0x10*t)+(offset)))

#define R_TMDMASEL	V_TIMER(0, 0x0c) /* dma or interrupt mode selection */

#define V_TIMER_0(offset)	V_TIMER(0, offset)
#define V_TIMER_1(offset)	V_TIMER(1, offset)
#define V_TIMER_2(offset)	V_TIMER(2, offset)

#define R_TMCON0		V_TIMER_0(0x00)
#define R_TMDATA0		V_TIMER_0(0x04)
#define R_TMCNT0		V_TIMER_0(0x08)

#define R_TMCON1		V_TIMER_1(0x00)
#define R_TMDATA1		V_TIMER_1(0x04)
#define R_TMCNT1		V_TIMER_1(0x08)

#define R_TMCON2		V_TIMER_2(0x00)
#define R_TMDATA2		V_TIMER_2(0x04)
#define R_TMCNT2		V_TIMER_2(0x08)

#define R_TMSTATUS		V_TIMER_0(0x30)

#define TMCON_MUX04		(0x0 << 2)
#define TMCON_MUX08		(0x1 << 2)
#define TMCON_MUX16		(0x2 << 2)
#define TMCON_MUX32		(0x3 << 2)

#define TMCON_INT_DMA_EN	(0x1 << 1)
#define TMCON_RUN		(0x1)

#define TMDATA_PRES(x)	(x > 0) ? ((x & 0xFF) << 16) : 1

#endif
