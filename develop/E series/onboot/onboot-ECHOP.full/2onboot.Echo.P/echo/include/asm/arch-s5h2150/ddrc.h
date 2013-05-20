/*
 * linux/include/asm-arm/arch-s5h2150/ddrc.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */
#ifndef _S5H2150_DDRC_H_
#define _S5H2150_DDRC_H_

#define V_DDRC(offset)  (*(volatile unsigned *)(V_DDRC_BASE+(offset)))

#define O_DDR_REG_0	(0x00)
#define O_DDR_REG_1	(0x08)
#define O_DDR_REG_2	(0x10)
#define O_DDR_REG_3	(0x18)
#define O_DDR_REG_4	(0x20)
#define O_DDR_REG_5	(0x28)
#define O_DDR_REG_6	(0x30)
#define O_DDR_REG_7	(0x40)
#define O_DDR_REG_8	(0x48)
#define O_DDR_REG_9	(0x50)
#define O_DDR_REG_10	(0x80)
#define O_DDR_REG_11	(0x88)
#define O_DDR_REG_12	(0x90)

#define	R_DDR_REG_0	V_DDRC(O_DDR_REG_0)
#define	R_DDR_REG_1	V_DDRC(O_DDR_REG_1)
#define	R_DDR_REG_2	V_DDRC(O_DDR_REG_2)
#define	R_DDR_REG_3	V_DDRC(O_DDR_REG_3)
#define	R_DDR_REG_4	V_DDRC(O_DDR_REG_4)
#define	R_DDR_REG_5	V_DDRC(O_DDR_REG_5)
#define	R_DDR_REG_6	V_DDRC(O_DDR_REG_6)
#define	R_DDR_REG_7	V_DDRC(O_DDR_REG_7)
#define	R_DDR_REG_8	V_DDRC(O_DDR_REG_8)
#define	R_DDR_REG_9	V_DDRC(O_DDR_REG_9)
#define	R_DDR_REG_10	V_DDRC(O_DDR_REG_10)
#define	R_DDR_REG_11	V_DDRC(O_DDR_REG_11)
#define	R_DDR_REG_12	V_DDRC(O_DDR_REG_12)

#endif
