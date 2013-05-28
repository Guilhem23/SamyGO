/*
 * linux/include/asm-arm/arch-sdp/xmif.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */
#ifndef _SDP75_XMIF_H_
#define _SDP75_XMIF_H_

#define V_XMIF_W(offset)  (*(volatile unsigned *)(V_XMIF_W_BASE+(offset)))
#define V_XMIF_E(offset)  (*(volatile unsigned *)(V_XMIF_E_BASE+(offset)))

#define O_DDR2C0		(0x00)
#define O_DDR2C1		(0x08)
#define O_DDR2C2		(0x10)
#define O_DDR2C3		(0x18)
#define O_DDR2C4		(0x20)
#define O_DDR2C5		(0x28)
#define O_DDR2C6		(0x30)
#define O_DDR2C7		(0x40)
#define O_DDR2C8		(0x48)
#define O_DDR2C9		(0x50)
#define O_SCH0			(0x80)
#define O_SCH1			(0x88)
#define O_SCH2			(0x90)
#define O_SCH3                  (0x98)
#define O_CTRL                  (0xC0)

#define R_W_DDR2C0		V_XMIF_W(O_DDR2C0)
#define R_W_DDR2C1		V_XMIF_W(O_DDR2C1)
#define R_W_DDR2C2		V_XMIF_W(O_DDR2C2)
#define R_W_DDR2C3		V_XMIF_W(O_DDR2C3)
#define R_W_DDR2C4		V_XMIF_W(O_DDR2C4)
#define R_W_DDR2C5		V_XMIF_W(O_DDR2C5)
#define R_W_DDR2C6		V_XMIF_W(O_DDR2C6)
#define R_W_DDR2C7		V_XMIF_W(O_DDR2C7)
#define R_W_DDR2C8		V_XMIF_W(O_DDR2C8)
#define R_W_DDR2C9		V_XMIF_W(O_DDR2C9)
#define R_W_SCH0		V_XMIF_W(O_SCH0	)
#define R_W_SCH1		V_XMIF_W(O_SCH1	)
#define R_W_SCH2		V_XMIF_W(O_SCH2	)
#define R_W_SCH3		V_XMIF_W(O_SCH3 )
#define R_W_CTRL		V_XMIF_W(O_CTRL )

#define R_E_DDR2C0		V_XMIF_E(O_DDR2C0)
#define R_E_DDR2C1		V_XMIF_E(O_DDR2C1)
#define R_E_DDR2C2		V_XMIF_E(O_DDR2C2)
#define R_E_DDR2C3		V_XMIF_E(O_DDR2C3)
#define R_E_DDR2C4		V_XMIF_E(O_DDR2C4)
#define R_E_DDR2C5		V_XMIF_E(O_DDR2C5)
#define R_E_DDR2C6		V_XMIF_E(O_DDR2C6)
#define R_E_DDR2C7		V_XMIF_E(O_DDR2C7)
#define R_E_DDR2C8		V_XMIF_E(O_DDR2C8)
#define R_E_DDR2C9		V_XMIF_E(O_DDR2C9)
#define R_E_SCH0		V_XMIF_E(O_SCH0	)
#define R_E_SCH1		V_XMIF_E(O_SCH1	)
#define R_E_SCH2		V_XMIF_E(O_SCH2	)
#define R_E_SCH3		V_XMIF_E(O_SCH3 )
#define R_E_CTRL		V_XMIF_E(O_CTRL )


#endif /* endif _SDP75_XMIF_H_ */
