/**************************************************
 * SDP1001 basics and PMU
 * This file should be embedded within onboot.S
 **************************************************/
/*
 * 2010/9/10a : apply to eval/mass modify by tukho.kim(#0717), offer by bckim.kim(#0663)
 * 2010/9/10c : apply to eval/mass modify by tukho.kim(#0717), offer by bckim.kim(#0663)
 * 2010/10/7a : revision  modify by tukho.kim(#0717), offer by bckim.kim(#0663)
 */

#include "sdp_macros.S"

	.macro wait_mask, tmp, base, offset, mask
1:	ldr	\tmp, [\base, \offset]
	ands	\tmp, \tmp, \mask
	bne	1b
	.endm

	b	ddr_a_setup

wait_cmd:
	ldr		r1, [r0, #0x040]
	ands	r1, r1, #0x80000000
	bne		wait_cmd
	mov		pc, lr

ddr_a_setup:

@ auto generated. 2011-08-08 10:54:31


	ldr	r0, =0x30408000
	rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	rsetl	r0, #0x540, r1, 0x32E5D7AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:68.7ohm, ODT pull-up:68.7ohm
	rsetl	r0, #0x544, r1, 0x00000000
	rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	rsetl	r0, #0x0C0, r1, 0x000000C8	@ 1 uSec = p_clk period(200MHz) * 0xC8(200)
	rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	rsetl	r0, #0x0CC, r1, 0x00000014	@ 100 nSec = p_clk (200MHz) * 0x14(20)
	rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable
	rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization
	rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec@333MHz), tPLLLOCK=0x8214(100uSec@333MHz)
	rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	@ 0x00000007 wait for PHY/PLL initialization done + calibration done
1:	ldr	r1, [r0, #0x410]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000007
	bne	1b
	rsetl	r0, #0x044, r1, 0x00000001	@ power up sequence start
	@ wait for power up sequence done
2:	ldr	r1, [r0, #0x048]
	and	r1, r1, #0x00000001
	cmp	r1, #0x00000001
	bne	2b
	rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
	rsetl	r0, #0x0D8, r1, 0x000000F0	@ tRFC=149.5(160nSec 2Gbit), 280.3(300ns 4Gbit)
	rsetl	r0, #0x0DC, r1, 0x0000000B	@ tRP=11(13.75nSec)
	rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	rsetl	r0, #0x0E8, r1, 0x0000000B	@ tCL=11
	rsetl	r0, #0x0EC, r1, 0x00000008	@ tCWL=8
	rsetl	r0, #0x0F0, r1, 0x0000001C	@ tRAS=28(35nSec)
	rsetl	r0, #0x0F4, r1, 0x00000027	@ tRC=39(48.75nSec)
	rsetl	r0, #0x0F8, r1, 0x0000000B	@ tRCD=11(13.75nSec)
	rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=6(7.5nSec)
	rsetl	r0, #0x100, r1, 0x00000006	@ tRTP=6(7.5nSec)
	rsetl	r0, #0x104, r1, 0x0000000C	@ tWR=12(15nSec)
	rsetl	r0, #0x108, r1, 0x00000006	@ tWTR=6(7.5nSec)
	rsetl	r0, #0x10C, r1, 0x00000200	@ tEXSR=512(exit self refresh to fir valid command)
	rsetl	r0, #0x110, r1, 0x00000005	@ tXP=4.8(exit power down to first valid command)
	rsetl	r0, #0x120, r1, 0x00000001	@ tDQS=1
	rsetl	r0, #0x0E0, r1, 0x00000004	@ tRTW=3 (Valencia Problem)
	rsetl	r0, #0x124, r1, 0x00000008	@ tCKSRE=8
	rsetl	r0, #0x128, r1, 0x00000008	@ tCKSRX=8
	rsetl	r0, #0x130, r1, 0x0000000C	@ tMOD=12
	rsetl	r0, #0x12C, r1, 0x00000004	@ tCKE=4
	rsetl	r0, #0x134, r1, 0x0000005E	@ tRSTL=67(100nSec) (synopsys)
	rsetl	r0, #0x118, r1, 0x00000034	@ tZQCS=51.2
	rsetl	r0, #0x138, r1, 0x00000200	@ tZQCL(tZQinit)=512 (synopsys)
	rsetl	r0, #0x114, r1, 0x00000014	@ tXPDLL=19.2
	rsetl	r0, #0x11C, r1, 0x00000005	@ tZQCSI=5 (synopsys)
	rsetl	r0, #0x090, r1, 0x00111150	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=2
	rsetl	r0, #0x000, r1, 0x00000021	@ SCFG
	rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	@ wait for command operation done
3:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	3b
	rsetl	r0, #0x040, r1, 0x80F40183	@ MCMD. MR2 : CWL=7
	@ wait for command operation done
4:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	4b
	rsetl	r0, #0x040, r1, 0x80F60003	@ MCMD. MR3
5:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	5b
	rsetl	r0, #0x040, r1, 0x80F20043	@ MR1 : rzq/4, DLL Enable, Ouptut Driver Imp rZQ/6
	@ MCMD. MR1 : DLL O, Write Leveling X, Output Driver Imp. = RZQ/7
6:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	6b
	rsetl	r0, #0x040, r1, 0x80F0F713	@ MCMD. MR0 (DLL reset)
	@ wait for command operation done
7:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	7b
	rsetl	r0, #0x040, r1, 0x80F00005	@ MCMD. ZQ calibration long
	@ wait for command operation done
8:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	8b
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to CFG mode
	@ wait for command operation done
9:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	9b
	rsetl	r0, #0x4AC, r1, 0x02111100
	rsetl	r0, #0x42C, r1, 0x0000000a
	rsetl	r0, #0x484, r1, 0xBFFB0000	@ Trainning OFF
	ldr	r2, =0xFFFF0000
10:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	10b
	rsetl	r0, #0x090, r1, 0x00111150	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=5,qse_alen=0
	rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	rsetl	r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]
#if defined(BOARD_Pre1)
	rsetl	r0, #0x5CC, r1, 0x0A2481C8	@ wdq bdl      
	rsetl	r0, #0x5D0, r1, 0x003CB34A	@ wdq/wdqs bdl 
	rsetl	r0, #0x5D4, r1, 0x00000000   @output enable
	rsetl	r0, #0x5D8, r1, 0x06082003   @rdq bdl
	rsetl	r0, #0x5DC, r1, 0x00005184   @rdq/rdqs bdl
	rsetl	r0, #0x5E4, r1, 0x00171713   @rdqs/wdq lcdl 	
	rsetl	r0, #0x5E8, r1, 0x00000003   @gdqs lcdl

	rsetl	r0, #0x60C, r1, 0x07106184   @wdq bdl
	rsetl	r0, #0x610, r1, 0x003C8207	@ wdq/wdqs bdl 
	rsetl	r0, #0x614, r1, 0x00000000   @output enable
	rsetl	r0, #0x618, r1, 0x06002001   @rdq bdl
	rsetl	r0, #0x61C, r1, 0x00002181   @rdq/rdqs bdl
	rsetl	r0, #0x624, r1, 0x00151513   @rdqs/wdq lcdl	
	rsetl	r0, #0x628, r1, 0x00000006   @gdqs lcdl

	rsetl	r0, #0x64C, r1, 0x0B2CA288	@ wdq bdl      
	rsetl	r0, #0x650, r1, 0x0040B2CC	@ wdq/wdqs bdl 
	rsetl	r0, #0x654, r1, 0x00000000   @output enable
	rsetl	r0, #0x658, r1, 0x040C1040	@ rdq bdl      
	rsetl	r0, #0x65C, r1, 0x000030C3	@ rdq/rdqs bdl 
	rsetl	r0, #0x664, r1, 0x00151513   @rdqs/wdq lcdl	
	rsetl	r0, #0x668, r1, 0x00000004   @gdqs lcdl

	rsetl	r0, #0x68C, r1, 0x09208208   @wdq bdl
	rsetl	r0, #0x690, r1, 0x003C7207	@ wdq/wdqs bdl 
	rsetl	r0, #0x694, r1, 0x00000000   @output enable
	rsetl	r0, #0x698, r1, 0x03001000   @rdq bdl
	rsetl	r0, #0x69C, r1, 0x000000C0	@ rdq/rdqs bdl 
	rsetl	r0, #0x6A4, r1, 0x00181815   @rdqs/wdq lcdl	
	rsetl	r0, #0x6A8, r1, 0x00000003	@ gdqs lcdl    
#else @ Pre2, DV
	rsetl	r0, #0x5CC, r1, 0x08207147	@ wdq bdl      
	rsetl	r0, #0x5D0, r1, 0x003092C9	@ wdq/wdqs bdl 
	rsetl	r0, #0x5D4, r1, 0x00000000   @output enable
	rsetl	r0, #0x5D8, r1, 0x080C4085	@ rdq bdl      
	rsetl	r0, #0x5DC, r1, 0x00008206	@ rdq/rdqs bdl 
	rsetl	r0, #0x5E4, r1, 0x00181815	@ rdqs/wdq lcdl
	rsetl	r0, #0x5E8, r1, 0x00000004	@ gdqs lcdl   

	rsetl	r0, #0x60C, r1, 0x081861C5	@ wdq bdl      
	rsetl	r0, #0x610, r1, 0x00408247	@ wdq/wdqs bdl 
	rsetl	r0, #0x614, r1, 0x00000000   @output enable
	rsetl	r0, #0x618, r1, 0x080C5084	@ rdq bdl      
	rsetl	r0, #0x61C, r1, 0x00005203	@ rdq/rdqs bdl 
	rsetl	r0, #0x624, r1, 0x00171716	@ rdqs/wdq lcdl
	rsetl	r0, #0x628, r1, 0x00000004	@ gdqs lcdl    

	rsetl	r0, #0x64C, r1, 0x0A288249	@ wdq bdl      
	rsetl	r0, #0x650, r1, 0x002CA24B	@ wdq/wdqs bdl 
	rsetl	r0, #0x654, r1, 0x00000000   @output enable
	rsetl	r0, #0x658, r1, 0x061020C3	@ rdq bdl      
	rsetl	r0, #0x65C, r1, 0x00005105	@ rdq/rdqs bdl 
	rsetl	r0, #0x664, r1, 0x00181815	@ rdqs/wdq lcdl
	rsetl	r0, #0x668, r1, 0x00000005	@ gdqs lcdl   

	rsetl	r0, #0x68C, r1, 0x091C9208	@ wdq bdl      i
	rsetl	r0, #0x690, r1, 0x00347207	@ wdq/wdqs bdl 
	rsetl	r0, #0x694, r1, 0x00000000   @output enable
	rsetl	r0, #0x698, r1, 0x05042081	@ rdq bdl      
	rsetl	r0, #0x69C, r1, 0x00000100	@ rdq/rdqs bdl 
	rsetl	r0, #0x6A4, r1, 0x00181816	@ rdqs/wdq lcdl
	rsetl	r0, #0x6A8, r1, 0x00000002	@ gdqs lcdl    
#endif	
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to active mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	11b


	ldr	r0, =0x30400000
	rsetl	r0, #0x008, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x00C, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x010, r1, 0x02010201	@ max timeout for p1 th0
	rsetl	r0, #0x014, r1, 0x00000000	@ max timeout for p1 th0
	rsetl	r0, #0x018, r1, 0x00000000	@ max timeout for p1 th1
	rsetl	r0, #0x01C, r1, 0x00000000	@ max timeout for p1 th1
	rsetl	r0, #0x020, r1, 0x02410241	@ max timeout for p1 th2
	rsetl	r0, #0x024, r1, 0x00000000	@ max timeout for p1 th2
	rsetl	r0, #0x028, r1, 0x00010001	@ max timeout for p1 th3
	rsetl	r0, #0x02C, r1, 0x00010001	@ max timeout for p1 th3


ddr_b_setup:
/* DDR AB setup ************************************/

@ auto generated. 2011-08-08 13:12:38


	ldr	r0, =0x30808000
	rsetl	r0, #0x000, r1, 0x00000030	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1)

	ldr	r0, =0x30818000
	rsetl	r0, #0x000, r1, 0x00000038	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1, [3] slave_mode : 1'b1 )

	ldr	r0, =0x30808000
	rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	rsetl	r0, #0x540, r1, 0x32E5D7AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:100.7ohm, ODT pull-up:100.7ohm 
	rsetl	r0, #0x544, r1, 0x00000000
	rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	rsetl	r0, #0x0C0, r1, 0x000000E9	@ 1 uSec = p_clk period(233MHz) * 0xe9(233MHz)
	rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	rsetl	r0, #0x0CC, r1, 0x00000017	@ 100 nSec = p_clk (233MHz) * 0x10(16)
	rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable

	ldr	r0, =0x30818000
	rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	rsetl	r0, #0x540, r1, 0x32E5D7AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:100.7ohm, ODT pull-up:100.7ohm 
	rsetl	r0, #0x544, r1, 0x00000000
	rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	rsetl	r0, #0x0C0, r1, 0x000000E9	@ 1 uSec = p_clk period(233MHz) * 0xe9(233MHz)
	rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	rsetl	r0, #0x0CC, r1, 0x00000017	@ 100 nSec = p_clk (233MHz) * 0x10(16)
	rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable

	ldr	r0, =0x30808000
	rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization on for master
	rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec at 333MHz), tPLLLOCK=0x8214(100uSec at 333MHz)
	rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	rsetl	r0, #0x404, r1, 0x0100C460	@ bypass PHY initialization off for master

	ldr	r0, =0x30818000
	rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization on for slave
	rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec at 333MHz), tPLLLOCK=0x8214(100uSec at 333MHz)
	rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	rsetl	r0, #0x404, r1, 0x0100C460	@ bypass PHY initialization off for master
	rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization

	ldr	r0, =0x30808000
	rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	ldr	r0, =0x30818000
1:	ldr	r1, [r0, #0x404]
	and	r1, r1, #0x00000001
	cmp	r1, #0x00000000
	bne	1b
	ldr	r0, =0x30808000
2:	ldr	r1, [r0, #0x404]
	and	r1, r1, #0x00000001
	cmp	r1, #0x00000000
	bne	2b
	@ wait for PHY/PLL initialization done + calibration done
3:	ldr	r1, [r0, #0x410]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000007
	bne	3b
	rsetl	r0, #0x044, r1, 0x00000001	@ power up sequence start
	@ wait for power up sequence done
4:	ldr	r1, [r0, #0x048]
	and	r1, r1, #0x00000001
	cmp	r1, #0x00000001
	bne	4b
	rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
	rsetl	r0, #0x0D8, r1, 0x00000096	@ tRFC=149.5(160nSec 2Gbit), tRFC=280.37(300nSec 4Gbit)  보드에 따라 다르게 적용
	rsetl	r0, #0x0DC, r1, 0x0000000D	@ tRP=13(13.91nSec)
	rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	rsetl	r0, #0x0E8, r1, 0x0000000D	@ tCL=13
	rsetl	r0, #0x0EC, r1, 0x00000009	@ tCWL=9
	rsetl	r0, #0x0F0, r1, 0x00000020	@ tRAS=31.77(34nSec)
	rsetl	r0, #0x0F4, r1, 0x0000002D	@ tRC=44.77(47.91nSec)
	rsetl	r0, #0x0F8, r1, 0x0000000D	@ tRCD=13(13.91nSec)
	rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=6(7.5nSec)
	rsetl	r0, #0x100, r1, 0x00000007	@ tRTP=7(7.5nSec)
	rsetl	r0, #0x104, r1, 0x0000000E	@ tWR=14.01(15nSec)
	rsetl	r0, #0x108, r1, 0x00000007	@ tWTR=7(7.5nSec)
	rsetl	r0, #0x10C, r1, 0x00000200	@ tEXSR=512(exit self refresh to fir valid command)
	rsetl	r0, #0x110, r1, 0x00000006	@ tXP=5.6(exit power down to first valid command)
	rsetl	r0, #0x120, r1, 0x00000001	@ tDQS=1
	rsetl	r0, #0x0E0, r1, 0x00000004	@ tRTW=4 (Valencia Problem)
	rsetl	r0, #0x124, r1, 0x0000000A	@ tCKSRE=9.34
	rsetl	r0, #0x128, r1, 0x0000000A	@ tCKSRX=9.34
	rsetl	r0, #0x130, r1, 0x0000000E	@ tMOD=14.01
	rsetl	r0, #0x12C, r1, 0x00000005	@ tCKE=4.67
	rsetl	r0, #0x134, r1, 0x0000005E	@ tRSTL=67(100nSec)
	rsetl	r0, #0x118, r1, 0x0000004B	@ tZQCS=74.76
	rsetl	r0, #0x138, r1, 0x00000200	@ tZQCL(tZQinit)=512
	rsetl	r0, #0x114, r1, 0x00000017	@ tXPDLL=22.42
	rsetl	r0, #0x11C, r1, 0x00000005	@ tZQCSI=5
	rsetl	r0, #0x090, r1, 0x00111132	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=3
	rsetl	r0, #0x000, r1, 0x00000031	@ SCFG

	ldr	r0, =0x30818000
	rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
	rsetl	r0, #0x0D8, r1, 0x00000096	@ tRFC=107(160nSec)  보드에 따라 다르게 적용
	rsetl	r0, #0x0DC, r1, 0x0000000D	@ tRP=9(13.5nSec)
	rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	rsetl	r0, #0x0E8, r1, 0x0000000D	@ tCL=13
	rsetl	r0, #0x0EC, r1, 0x00000009	@ tCWL=9
	rsetl	r0, #0x0F0, r1, 0x00000020	@ tRAS=31.77(34nSec)
	rsetl	r0, #0x0F4, r1, 0x0000002D	@ tRC=44.77(47.91nSec)
	rsetl	r0, #0x0F8, r1, 0x0000000D	@ tRCD=13(13.91nSec)
	rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=5(7.5nSec)
	rsetl	r0, #0x100, r1, 0x00000007	@ tRTP=7(7.5nSec)
	rsetl	r0, #0x104, r1, 0x0000000E	@ tWR=14(15nSec)
	rsetl	r0, #0x108, r1, 0x00000007	@ tWTR=7(7.5nSec)
	rsetl	r0, #0x10C, r1, 0x00000200	@ tEXSR=512(exit self refresh to fir valid command)
	rsetl	r0, #0x110, r1, 0x00000006	@ tXP=5.6(exit power down to first valid command)
	rsetl	r0, #0x120, r1, 0x00000001	@ tDQS=1
	rsetl	r0, #0x0E0, r1, 0x00000004	@ tRTW=4 (Valencia Problem)
	rsetl	r0, #0x124, r1, 0x0000000A	@ tCKSRE=9.34
	rsetl	r0, #0x128, r1, 0x0000000A	@ tCKSRX=9.34
	rsetl	r0, #0x130, r1, 0x0000000E	@ tMOD=14.01
	rsetl	r0, #0x12C, r1, 0x00000005	@ tCKE=4.67
	rsetl	r0, #0x134, r1, 0x0000005E	@ tRSTL=67(100nSec)
	rsetl	r0, #0x118, r1, 0x0000004B	@ tZQCS=74.76
	rsetl	r0, #0x138, r1, 0x00000200	@ tZQCL(tZQinit)=512
	rsetl	r0, #0x114, r1, 0x00000017	@ tXPDLL=22.42
	rsetl	r0, #0x11C, r1, 0x00000005	@ tZQCSI=5
	rsetl	r0, #0x090, r1, 0x00111132	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=3
	rsetl	r0, #0x000, r1, 0x00000039	@ SCFG

	ldr	r0, =0x30808000
	rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	@ wait for command operation done
5:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	5b
	rsetl	r0, #0x040, r1, 0x80F40203	@ MCMD. MR2 : CWL=7
	@ wait for command operation done
6:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	6b
	rsetl	r0, #0x040, r1, 0x80F60003	@ MCMD. MR3
	@ wait for command operation done
7:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	7b
	rsetl	r0, #0x040, r1, 0x80F20043	@ rzq/4 MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/6
	@ wait for command operation done
8:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	8b
	rsetl	r0, #0x040, r1, 0x80F0F153	@ MCMD. MR0 (DLL reset)
	@ wait for command operation done
9:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	9b
	rsetl	r0, #0x040, r1, 0x80F00005	@ MCMD. ZQ calibration long
	@ wait for command operation done
10:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	10b

	ldr	r0, =0x30818000
	rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	@ wait for command operation done
11:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	11b
	rsetl	r0, #0x040, r1, 0x80F40203	@ MCMD. MR2 : CWL=7
	@ wait for command operation done
12:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	12b
	rsetl	r0, #0x040, r1, 0x80F60003	@ MCMD. MR3
	@ wait for command operation done
13:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	13b
	rsetl	r0, #0x040, r1, 0x80F20043	@ rzq/4 MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/7
	@ wait for command operation done
14:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	14b
	rsetl	r0, #0x040, r1, 0x80F0F153	@ MCMD. MR0 (DLL reset)
	@ wait for command operation done
15:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	15b
	rsetl	r0, #0x040, r1, 0x80F00005	@ MCMD. ZQ calibration long
	@ wait for command operation done
16:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	16b

	ldr	r0, =0x30808000
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to CFG mode
	@ wait for CFG mode
17:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	17b

	ldr	r0, =0x30818000
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to CFG mode
	@ wait for CFG mode
18:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	18b

	ldr	r0, =0x30808000
	rsetl	r0, #0x42C, r1, 0x0000040F	@ CK0 CLK Delay 15/CK1 CLK Delay 16
	rsetl	r0, #0x4AC, r1, 0x02111100

	ldr	r0, =0x30818000
	rsetl	r0, #0x4AC, r1, 0x02111100

	ldr	r0, =0x30808000
	rsetl	r0, #0x484, r1, 0x24530000	@ Trainning On(ST3, ST8 Disabled, related to write-leveling ON)

	ldr	r0, =0x30818000
	rsetl	r0, #0x484, r1, 0x24530000	@ Trainning On(ST3, ST8 Disabled, related to write-leveling ON)
	@  wait for all SM sequence done
	ldr	r0, =0x30808000
	ldr	r2, =0xFFFF0000
19:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	19b
	@  wait for all SM sequence done
	ldr	r0, =0x30818000
	ldr	r2, =0xFFFF0000
20:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	20b
#if defined(BOARD_Pre1)
	@ Write LCDL '-8' shift
	ldr	r0, =0x30808000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000008
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Read LCDL '5' shift
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00050500
	add	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Write LCDL '-8' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000008
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Read LCDL '5' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00050500
	add	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Write LCDL '-8' shift
	ldr	r0, =0x30818000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000008
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Read LCDL '5' shift
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00050500
	add	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Write LCDL '-8' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000008
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Read LCDL '5' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00050500
#else @ Pre2, DV
	@ Write LCDL '-4' shift
	ldr	r0, =0x30808000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000004
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Read LCDL '0' shift
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000000
	add	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Write LCDL '-4' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000004
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Read LCDL '0' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000000
	add	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Write LCDL '-4' shift
	ldr	r0, =0x30818000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000004
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Read LCDL '0' shift
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000000
	add	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ Write LCDL '-4' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000004
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	@ Read LCDL '0' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000000
#endif
	add	r1, r1, r2
	str	r1, [r0, #0x624]

	ldr	r0, =0x30808000
	rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	rsetl	r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI

	ldr	r0, =0x30818000
	rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	rsetl	r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI

	ldr	r0, =0x30808000
	rsetl	r0, #0x040, r1, 0x80F20043	@ rzq/4 MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/6

	ldr	r0, =0x30818000
	rsetl	r0, #0x040, r1, 0x80F20043	@ rzq/4 MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/6
	ldr	r0, =0x30808000
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]
	ldr	r0, =0x30818000
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]

	ldr	r0, =0x30808000
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	@ wait for Access mode
21:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	21b

	ldr	r0, =0x30818000
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to access mode
	@ wait for Access mode
22:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	22b


	ldr	r0, =0x30800000
	rsetl	r0, #0x008, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x00C, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x010, r1, 0x00000000	@ max timeout for p1 th0
	rsetl	r0, #0x014, r1, 0x00810081	@ max timeout for p1 th0
	rsetl	r0, #0x018, r1, 0x0FF10FF1	@ max timeout for p1 th1
	rsetl	r0, #0x01C, r1, 0x0FF10FF1	@ max timeout for p1 th1
	rsetl	r0, #0x020, r1, 0x02010201	@ max timeout for p1 th2
	rsetl	r0, #0x024, r1, 0x00000000	@ max timeout for p1 th2
	rsetl	r0, #0x028, r1, 0x00000000	@ max timeout for p1 th3
	rsetl	r0, #0x02C, r1, 0x00000000	@ max timeout for p1 th3


	ldr	r0, =0x30810000
	rsetl	r0, #0x008, r1, 0x07010701	@ max timeout for p0
	rsetl	r0, #0x00C, r1, 0x07010701	@ max timeout for p0
	rsetl	r0, #0x010, r1, 0x02010201	@ max timeout for p1 th0
	rsetl	r0, #0x014, r1, 0x02010201	@ max timeout for p1 th0
	rsetl	r0, #0x018, r1, 0x02010201	@ max timeout for p1 th1
	rsetl	r0, #0x01C, r1, 0x02010201	@ max timeout for p1 th1
	rsetl	r0, #0x020, r1, 0x02010201	@ max timeout for p1 th2
	rsetl	r0, #0x024, r1, 0x02010201	@ max timeout for p1 th2
	rsetl	r0, #0x028, r1, 0x02010201	@ max timeout for p1 th3
	rsetl	r0, #0x02C, r1, 0x02010201	@ max timeout for p1 th3


	@XMIF setting
	ldr	r0, =0x30650000
	rsetl   r0, #0x000, r1, 0x01213233
	rsetl	r0, #0x004, r1, 0x0D132220

