/*
 * linux/include/asm-arm/arch-sdp75/gpio.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_GPIO_H_
#define _SDP75_GPIO_H_

/*-----------------------------------------------------*/
/* GPIO ports */
#define V_GPIO_PORT(offset)  (*(volatile unsigned *)(V_GPIO_BASE+(offset)))

#define O_PCON0			(0x00)
#define O_PCON1			(0x04)
#define O_PCON2			(0x08)
#define O_PCON3			(0x0C)
#define O_PCON4			(0x10)
#define O_PCON5			(0x14)
#define O_PCON6			(0x18)
#define O_PCON7			(0x1C)
#define O_PCON8			(0x20)
#define O_PCON9			(0x24)

#define O_PWDAT0		(0x28)
#define O_PWDAT1		(0x2C)
#define O_PWDAT2		(0x30)
#define O_PWDAT3		(0x34)
#define O_PWDAT4		(0x38)
#define O_PWDAT5		(0x3C)
#define O_PWDAT6		(0x40)
#define O_PWDAT7		(0x44)
#define O_PWDAT8		(0x48)
#define O_PWDAT9		(0x4C)

#define O_PULL00		(0x78)
#define O_PULL01		(0x7C)
#define O_PULL02		(0x80)
#define O_PULL03		(0x84)
#define O_PULL04		(0x88)
#define O_PULL05		(0x8C)
#define O_PULL06		(0x90)
#define O_PULL07		(0x94)
#define O_PULL08		(0x98)
#define O_PULL09		(0x9C)
#define O_PULL10		(0xA0)
#define O_PULL11		(0xA4)
#define O_PULL12		(0xA8)
#define O_PULL13		(0xAC)
#define O_PULL14		(0xB0)

#define O_JTAG_SEL0		(0xB4)
#define O_JTAG_SEL1		(0xB8)
#define O_EXTINTR		(0xBC)
#define O_EXT_INT_SEL		(0xC0)
#define O_EXT_BYPASS		(0xC4)

// define for 'C'
#define R_PCON0			(O_PCON0)
#define R_PCON1			(O_PCON1)
#define R_PCON2			(O_PCON2)
#define R_PCON3			(O_PCON3)
#define R_PCON4			(O_PCON4)
#define R_PCON5			(O_PCON5)
#define R_PCON6			(O_PCON6)
#define R_PCON7			(O_PCON7)
#define R_PCON8			(O_PCON8)
#define R_PCON9			(O_PCON9)
                                                 
#define R_PWDAT0		(O_PWDAT0)
#define R_PWDAT1		(O_PWDAT1)
#define R_PWDAT2		(O_PWDAT2)
#define R_PWDAT3		(O_PWDAT3)
#define R_PWDAT4		(O_PWDAT4)
#define R_PWDAT5		(O_PWDAT5)
#define R_PWDAT6		(O_PWDAT6)
#define R_PWDAT7		(O_PWDAT7)
#define R_PWDAT8		(O_PWDAT8)
#define R_PWDAT9		(O_PWDAT9)
                                                 
#define R_PULL00		(O_PULL00)
#define R_PULL01		(O_PULL01)
#define R_PULL02		(O_PULL02)
#define R_PULL03		(O_PULL03)
#define R_PULL04		(O_PULL04)
#define R_PULL05		(O_PULL05)
#define R_PULL06		(O_PULL06)
#define R_PULL07		(O_PULL07)
#define R_PULL08		(O_PULL08)
#define R_PULL09		(O_PULL09)
#define R_PULL10		(O_PULL10)
#define R_PULL11		(O_PULL11)
#define R_PULL12		(O_PULL12)
#define R_PULL13		(O_PULL13)
#define R_PULL14		(O_PULL14)
                                                 
#define R_JTAG_SEL0		(O_JTAG_SEL0)
#define R_JTAG_SEL1		(O_JTAG_SEL1)
#define R_EXTINTR		(O_EXTINTR)
#define R_EXT_INT_SEL		(O_EXT_INT_SEL)
#define R_EXT_BYPASS		(O_EXT_BYPASS)


#endif //_SDP75_GPIO_H_
