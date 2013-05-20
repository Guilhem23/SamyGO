/*
 * linux/include/asm-arm/arch-sdp/smc.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_SMC_H
#define  _SDP75_SMC_H

/* SMC Register */
#define V_SMC(offset)  (*(volatile unsigned *)(V_SMC_BASE+(offset)))

#endif //_SDP75_SMC_H
