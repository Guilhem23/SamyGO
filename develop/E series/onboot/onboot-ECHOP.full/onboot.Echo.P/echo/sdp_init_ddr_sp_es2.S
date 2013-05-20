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

	.ltorg

#ifndef AUTO_TRAIN
sw_training:
	mov	r4, #0x0
	str	r4, slice_num
	str	r4, left_right_index
//	mov	r4, #0x1
	str	r4, wr_index
	
//	r4 : slice_num
//	r5 : lcdl_base
//	r6 : ddr_base
1:
	ldr	r4, slice_num
	ldr	r5, lcdl_base
	ldr	r6, ddr_base
	ldr	r7, =0x5e4
	ldr	r8, =0x624
	cmp	r4, #0x0
	addeq	r5, r0, r7
	addeq	r6, r2, #0x00
	beq	2f
	cmp	r4, #0x1
	addeq	r5, r0, r8
	addeq	r6, r2, #0x04
	beq	2f
	cmp	r4, #0x2
	addeq	r5, r1, r7
	addeq	r6, r3, #0x00
	beq	2f
	cmp	r4, #0x3
	addeq	r5, r1, r8
	addeq	r6, r3, #0x04

2:
//save values to TSD SRAM
	ldr	r8, left_right_index
	cmp	r8, #0x0
	ldreq	r8, wr_index
	cmpeq	r8, #0x0
	ldreq	r8, save_value
	ldreq	r10, [r5]
	streq	r10, [r8], #0x4
	streq	r8, save_value
//save end

	str	r5, lcdl_base
	str	r6, ddr_base

	mov	r4, #0x1
	str	r4, loop_state

//	r4 : wr_index
//	r9 : left_right_index
//	r10 : loop_index
//	r11 : wdq_ref
//	r8 : rdqs_ref
//	r5 : lcdl_base(tmp)

	ldr	r4, wr_index
	ldr	r9, left_right_index
	ldr	r10, loop_index
	ldr	r11, wdq_ref
	ldr	r8, rdqs_ref
	cmp	r4, #0x0
	bne	3f
	cmp	r9, #0x0
	bne	10f
	ldr	r5, [r5]
	ubfx	r11, r5, #0x0, #0x8
	ubfx	r8, r5, #0x8, #0x8
	cmp	r11, #0x15
	movge	r11, #0x9
	sub	r10, r11, #0x1
	str	r11, wdq_ref
	str	r8, rdqs_ref
	b	11f
10:
	addne	r10, r11, #0x1
11:
	str	r10, loop_index
	b	4f
3:
	cmp	r9, #0x0
	ldreq	r5, [r5]
	ubfxeq	r11, r5, #0x0, #0x8
	ubfxeq	r8, r5, #0x8, #0x8
	subeq	r10, r8, #0x1
	streq	r11, wdq_ref
	streq	r8, rdqs_ref
	addne	r10, r8, #0x1
	str	r10, loop_index

//	r5 : loop_state
//	r4 : wr_index
//	r6 : lcdl_value(tmp)
//	r7 : lcdl_base
//	r8 : ddr_base
//	r9 : rdata0 (tmp)
//	r10 : loop_index
//	r11 : wdq_ref, rdqs_ref (tmp)
4:	
	ldr	r5, loop_state
	cmp	r4, #0x0
	ldreq	r11, rdqs_ref
	addeq	r6, r10, r11, lsl #16
	addeq	r6, r6, r11, lsl #8
	ldreq	r11, wdq_ref
	addne	r6, r11, r10, lsl #16
	addne	r6, r6, r10, lsl #8
	ldr	r7, lcdl_base
	ldr	r8, ddr_base
	str	r6, [r7]
	ldr	r9, =0xa55aa55a
	mov	r11, #0x0
10:
	str	r11, [r8]
	str	r9, [r8]
	ldr	r6, [r8]
	cmp	r6, r9
	bne	6f
	add	r11, r11, #0x1
	cmp	r11, #256  //traning number
	blt	10b
	
	cmp	r10, #0x0
	moveq	r5, #0x0
	streq	r5, loop_state
	beq	5f
	ldr	r9, left_right_index
	cmp	r9, #0x0
	addeq	r10, r10, #0x1
	subne	r10, r10, #0x1
	str	r10, loop_index
5:
	b	7f
6:	
	mov	r5, #0x0
	str	r5, loop_state
7:
	cmp	r5, #0x1
	beq	4b

//	r9 : left_right_index
//	r10 : loop_index (1st, tmp)
//	r10 : rdqs_ref(tmp)
//	r4 : wr_index
//	r11 : wdq_ref
//	r5 : left_value
//	r6 : center_value
//	r7 : right_value
//	r8 : tmp
8:
	ldr	r9, left_right_index
	ldr	r11, wdq_ref
	cmp	r9, #0x0
	addeq	r5, r10, #0x1
	streq	r5, left_value
	moveq	r9, #0x1
	beq	9f
	ldr	r5, left_value
//	ldr	r7, right_value
	sub	r7, r10, #0x1
	str	r7, right_value
	ldr	r10, rdqs_ref
	add	r6, r5, r7
	lsr	r6, r6, #0x1
	cmp	r4, #0x0
	moveq	r11, r6
//	subeq	r11, r6, #0x3
	streq	r11, wdq_ref
	movne	r10, r6
//	subne	r10, r6, #0x3
	strne	r10, rdqs_ref
	add	r8, r11, r10, lsl #8
	add	r8, r8, r10, lsl #16
	ldr	r10, lcdl_base
	str	r8, [r10]
	mov	r9, #0x0
	cmp	r4, #0x1

//save values to TSD SRAM
	ldreq	r8, save_value
	ldreq	r10, slice_num
	streq	r10, [r8], #0x4
	ldreq	r10, left_value
	streq	r10, [r8], #0x4
	ldreq	r10, right_value
	streq	r10, [r8], #0x4
	ldreq	r10, rdqs_ref
	streq	r10, [r8], #0x4
	ldreq	r10, wdq_ref
	streq	r10, [r8], #0x4
	streq	r8, save_value
//save end

	ldreq	r8, slice_num
	addeq	r8, r8, #0x1
	streq	r8, slice_num
	moveq	r4, #0x0
	movne	r4, #0x1
9:	
	str	r4, wr_index
	str	r9, left_right_index
	ldr	r4, slice_num
	cmp	r4, #0x4
	blt	1b
	
	mov	pc, lr
#endif	

.ltorg

slice_num:
.word	0x0
wr_index:
.word	0x1
left_right_index:
.word	0x0
lcdl_base:
.word	0x0
ddr_base:
.word	0x0
loop_state:
.word	0x1
wdq_ref:
.word	0x0
rdqs_ref:
.word	0x0
loop_index:
.word	0x0
lcdl_value:
.word	0x0
left_value:
.word	0x0
right_value:
.word	0x0
center_value:
.word	0x0
save_value:
.word	0x30100800

set_acparam:
	rsetl	r0, #0x4A8, r1, 0x00001C00	@ PPMCFG : rank 0 enabled
	rsetl	r0, #0x0D0, r1, 0x00000027	@ tREF = 3900nSec
	rsetl	r0, #0x0D4, r1, 0x00000004	@ tMRD=4
#if defined(BOARD_OD)
	ldr r3, =0x30408000
	cmp r0, r3
	rseteql	r0, #0x0D8, r1, 0x00000119  @ tRFC=149.5(160nSec 2Gbit), tRFC=280.37(300nSec 4Gbit)  보드에 따라 다르게 적용 
	rsetnel	r0, #0x0D8, r1, 0x00000096
#else @ PV
	rsetl	r0, #0x0D8, r1, 0x00000096	@ tRFC=149.5(160nSec 2Gbit), tRFC=280.37(300nSec 4Gbit)  보드에 따라 다르게 적용
#endif
	rsetl	r0, #0x0DC, r1, 0x0000000D	@ tRP=13(13.91nSec)
	rsetl	r0, #0x0E4, r1, 0x00000000	@ tAL=0
	rsetl	r0, #0x0E8, r1, 0x0000000D	@ tCL=13
	rsetl	r0, #0x0EC, r1, 0x00000009	@ tCWL=9
	rsetl	r0, #0x0F0, r1, 0x00000020	@ tRAS=31.77(34nSec)
	rsetl	r0, #0x0F4, r1, 0x0000002D	@ tRC=44.77(47.91nSec)
	rsetl	r0, #0x0F8, r1, 0x0000000D	@ tRCD=13(13.91nSec)
	rsetl	r0, #0x0FC, r1, 0x00000006	@ tRRD=6(6nSec)
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
	rsetl	r0, #0x090, r1, 0x00111132	@ dv_alat=1,dv_alen=1,dqe_alat=1,dqe_alen=1,qse_alat=3,qse_alen=2
	mov	pc, lr


	
wait_cmd:
	ldr		r1, [r0, #0x040]
	ands	r1, r1, #0x80000000
	bne		wait_cmd
	mov		pc, lr

ddr_a_setup:

@ auto generated. 2011-10-20 14:28:57


	ldr	r0, =0x30408000
	rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	rsetl	r0, #0x540, r1, 0x32E5D7AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:65.7ohm, ODT pull-up:65.7ohm 
	rsetl	r0, #0x544, r1, 0x00000000
	rsetl	r0, #0x31C, r1, 0x00005201	@ Static ODT
	rsetl	r0, #0x080, r1, 0x00040021	@ tFAW=5*RRD, DDR3_en, Open-page, BL8
	rsetl	r0, #0x08C, r1, 0x00000008	@ ODT : value driven for BL/2 cycles following a write,rank 0
	rsetl	r0, #0x0C0, r1, 0x000000E9	@ 1 uSec = p_clk period(233MHz) * 0xA6(233)
	rsetl	r0, #0x0C4, r1, 0x000000C8	@ 200(=0xC8)uSec for CKE enable
	rsetl	r0, #0x0CC, r1, 0x00000017	@ 100 nSec = p_clk (233MHz) * 0x10(16)
	rsetl	r0, #0x0C8, r1, 0x000001F4	@ 500(=0x1F4)uSec for RST enable
	rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization
	rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec@333MHz), tPLLLOCK=0x8214(100uSec@333MHz)
	rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	rsetl	r0, #0x40C, r1, 0x0000F142	@ DDR IO Mode, write leveling type 변경
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
	bl	set_acparam
	rsetl	r0, #0x000, r1, 0x00000021	@ SCFG
	rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	@ wait for command operation done
3:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	3b
	rsetl	r0, #0x040, r1, 0x80F40203	@ MCMD. MR2 : CWL=9
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
6:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	6b
	rsetl	r0, #0x040, r1, 0x80F0F153	@ MCMD. MR0 (DLL reset)
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
	rsetl	r0, #0x42C, r1, 0x00000008	@ CLK Dealy 8
	rsetl	r0, #0x4AC, r1, 0x02111100
	rsetl	r0, #0x48C, r1, 0x00000004	@ SNPS-set PUB_MR1 register with MR1 value from PCTL
	@rsetl	r0, #0x484, r1, 0x24530000	@ Trainning On(ST3, ST8 Disabled, related to write-leveling ON)
	
	rsetl	r0, #0x484, r1, 0x00030008	@ ST2 VT-Disable

18:	ldr	r1, [r0, #0x484]                
	and	r1, r1, #0x00070000             
	cmp	r1, #0x00070000                 
	bne	18b                             
	ldr 	r2, [r0, #0x5e4]                                        
	ldr 	r3, [r0, #0x624]                                        
	ldr 	r4, [r0, #0x664]                                        
	ldr 	r5, [r0, #0x6a4]                                        
	and	r2, r2, #0xFF
	and	r3, r3, #0xFF
	and	r4, r4, #0xFF
	and	r5, r5, #0xFF	
	cmp 	r2, r3                                                  
	movgt 	r6, r3                                                
	movle 	r6, r2                                                
	cmp 	r6, r4                                                  
	movgt 	r6, r4                                                
	cmp 	r6, r5                                                  
	movgt 	r6, r5                                                
                                                                  
	add	r5, r6,	r6                                          
	sub	r5, r5, #0x5                                        
	lsl	r5, r5, #0x18                                       
	ldr	r2, =0x00FFFFFF                                     
	orr	r5, r5, r2			@ &lcdl_max= ((&min_+&min_wdqd-0x5)<<18)|0x00ffffff
	ldr	r6, =0xFF0500FF			@ &lcdl_min= 0xff05
	ldr	r2, =0xFFFFFF00                                     
	orr	r7, r2, r4			@ &lcdl_shift= 0xfff0|&min_wdqd
	and	r8, r5, r6                                          
	and	r8, r8, r7			@ &pub_tr= &lcdl_maxdl_min&&lcdl_shift
	str	r8, [r0, #0x4B4]                                    
	@rsetl	r0, #0x484, r1, 0x24570000            
	rsetl	r0, #0x484, r1, 0x64570000      @ VT Compensation OFF
	
	ldr	r2, =0xFFFF0000
10:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	10b
	
	
	rsetl	r0, #0x300, r1, 0x000045D0	@ PHYPVTCFG
	rsetl	r0, #0x308, r1, 0x00000008	@ PHYTUPDON
	rsetl	r0, #0x30C, r1, 0x00000004	@ PHYTUPDDLY
	rsetl	r0, #0x310, r1, 0x00000008	@ PHTTUPDON
	rsetl	r0, #0x314, r1, 0x00000004	@ PVTTUPDDLY
	rsetl	r0, #0x318, r1, 0x00000000	@ PHYPVTUPDI
	rsetl	r0, #0x320, r1, 0x00000003	@ PHYTUPDWAIT
	rsetl	r0, #0x324, r1, 0x00000003	@ PVTTUPDWAIT
	rsetl	r0, #0x328, r1, 0x00000000	@ PVTUPDI
	
	
	
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

@ auto generated. 2011-10-20 14:33:25


	ldr	r0, =0x30808000
	rsetl	r0, #0x000, r1, 0x00000030	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1)

	ldr	r0, =0x30818000
	rsetl	r0, #0x000, r1, 0x00000038	@ SCFG ([5] rkinf_en : 1'b1, [4] dual_pctl_en : 1'b1, [3] slave_mode : 1'b1 )

	ldr	r0, =0x30808000
	rsetl	r0, #0x434, r1, 0x00001EE4	@ dqs_r. 344ohm
	rsetl	r0, #0x540, r1, 0x34A957AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:40.7ohm, ODT pull-up:4.7ohm 
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
	rsetl	r0, #0x540, r1, 0x32E5D7AF	@ zqcr0. drive pull-down:40.6ohm, drive pull-up:40.6ohm, ODT pull-down:65.7ohm, ODT pull-up:65.7ohm 
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
	rsetl	r0, #0x40C, r1, 0x0000F142	@ DDR IO Mode, write leveling type 변경

	ldr	r0, =0x30818000
	rsetl	r0, #0x404, r1, 0x0100C462	@ bypass PHY initialization on for slave
	rsetl	r0, #0x41C, r1, 0x3A424690	@ PHY_PTR0 : tPHYRST=0x10, tPLLPD=0x14D (1uSec at 333MHz)
	rsetl	r0, #0x420, r1, 0xB608065F	@ PHY_PTR1 : tPLLRST=0x3E7(3uSec at 333MHz), tPLLLOCK=0x8214(100uSec at 333MHz)
	rsetl	r0, #0x424, r1, 0x0010FFFF	@ PHY_PTR2 : default values
	rsetl	r0, #0x418, r1, 0x0001C000	@ PHY_PLLCR
	rsetl	r0, #0x404, r1, 0x0100C460	@ bypass PHY initialization off for master
	rsetl	r0, #0x404, r1, 0x0100C461	@ PHY initialization
	rsetl	r0, #0x40C, r1, 0x0000F142	@ DDR IO Mode, write leveling type 변경

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
	bl	set_acparam
	rsetl	r0, #0x000, r1, 0x00000031	@ SCFG

	ldr	r0, =0x30818000
	bl	set_acparam
	rsetl	r0, #0x000, r1, 0x00000039	@ SCFG

	ldr	r0, =0x30808000
	rsetl	r0, #0x040, r1, 0x84F00000	@ MCMD. NOP
	@ wait for command operation done
5:	ldr	r1, [r0, #0x040]
	and	r1, r1, #0x80000000
	cmp	r1, #0x00000000
	bne	5b
	rsetl	r0, #0x040, r1, 0x80F40203	@ MCMD. MR2 : CWL=9
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
	rsetl	r0, #0x040, r1, 0x80F40203	@ MCMD. MR2 : CWL=9
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
	rsetl	r0, #0x040, r1, 0x80F20043	@ rzq/4 MCMD. MR1 : DLL enable, Write Leveling disabled, Output Driver Imp. = RZQ/6
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
	rsetl	r0, #0x48C, r1, 0x00000004	@ SNPS-set PUB_MR1 register with MR1 value from PCTL
	@rsetl	r0, #0x484, r1, 0x24530000	@ Trainning On(ST3, ST8 Disabled, related to write-leveling ON)

	rsetl	r0, #0x484, r1, 0x00030008	@ ST2 VT-Disable

18:	ldr	r1, [r0, #0x484]
	and	r1, r1, #0x00070000
	cmp	r1, #0x00070000
	bne	18b
	
	@write
	ldr	r2, [r0, #0x5e4]
	ldr	r3, [r0, #0x624]
	and 	r2, r2, #0xFF
	and 	r3, r3, #0xFF
	cmp	r2, r3
	movgt	r4, r3
	movle	r4, r2

	add	r5, r4,	r4
	sub	r5, r5, #0x5
	lsl	r5, r5, #0x18
	ldr	r2, =0x00FFFFFF
	orr	r5, r5, r2			@ &lcdl_max= ((&min_wdqd+&min_wdqd-0x5)<<18)|0x00ffffff
	ldr	r6, =0xFF0500FF			@ &lcdl_min= 0xff0500ff
	ldr	r2, =0xFFFFFF00
	orr	r7, r2, r4			@ &lcdl_shift= 0xffffff00|&min_wdqd
	and	r8, r5, r6
	and	r8, r8, r7			@ &pub_tr= &lcdl_max&&lcdl_min&&lcdl_shift
	str	r8, [r0, #0x4B4]
	@rsetl	r0, #0x484, r1, 0x24570000
	rsetl	r0, #0x484, r1, 0x64570000      @ VT Compensation OFF

	ldr	r0, =0x30818000
	rsetl	r0, #0x48C, r1, 0x00000004	@ SNPS-set PUB_MR1 register with MR1 value from PCTL
	@rsetl	r0, #0x484, r1, 0x24530000	@ Trainning On(ST3, ST8 Disabled, related to write-leveling ON)

	rsetl	r0, #0x484, r1, 0x00030008	@ ST2 VT-Disable

18:	ldr	r1, [r0, #0x484]
	and	r1, r1, #0x00070000
	cmp	r1, #0x00070000
	bne	18b
	
	@write
	ldr	r2, [r0, #0x5e4]
	ldr	r3, [r0, #0x624]
	and 	r2, r2, #0xFF
	and 	r3, r3, #0xFF
	cmp	r2, r3
	movgt	r4, r3
	movle	r4, r2

	add	r5, r4,	r4
	sub	r5, r5, #0x5
	lsl	r5, r5, #0x18
	ldr	r2, =0x00FFFFFF
	orr	r5, r5, r2			@ &lcdl_max= ((&min_wdqd+&min_wdqd-0x5)<<18)|0x00ffffff
	ldr	r6, =0xFF0500FF			@ &lcdl_min= 0xff0500ff
	ldr	r2, =0xFFFFFF00
	orr	r7, r2, r4			@ &lcdl_shift= 0xffffff00|&min_wdqd
	and	r8, r5, r6
	and	r8, r8, r7			@ &pub_tr= &lcdl_max&&lcdl_min&&lcdl_shift
	str	r8, [r0, #0x4B4]
	
	@rsetl	r0, #0x484, r1, 0x24570000
	rsetl	r0, #0x484, r1, 0x64570000      @ VT Compensation OFF

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

#if 0
	ldr	r0, =0x304085e4
	ldr	r1, =0x3010080c
	ldr	r2, [r0]
	str	r2, [r1]
#endif	

#ifndef AUTO_TRAIN
	ldr	r0, =0x30408000
	ldr	r1, =0x30408080
	ldr	r2, =0x40000000
	ldr	r3, =0x40000008
	bl	sw_training
#if 0
	ldr	r0, =0x30100800
	ldr	r1, rdqs_ref
	ldr	r2, left_value
	ldr	r3, right_value
	str	r1, [r0]
	str	r2, [r0, #0x4]
	str	r3, [r0, #0x8]
#endif	

	ldr	r0, =0x30808000
	ldr	r1, =0x30818000
	ldr	r2, =0x80000000
	ldr	r3, =0xA0000000
	bl	sw_training
#endif

        @ DDR A Write LCDL '-1' shift
	ldr	r0, =0x30408000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]

	@ DDR A  Write LCDL '-1' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x624]

	@ DDR A  Write LCDL '-1' shift
	ldr	r1, [r0, #0x664]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x664]

	@ DDR A  Write LCDL '-1' shift
	ldr	r1, [r0, #0x6A4]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x6A4]

        
        @ DDR B Write LCDL '-1' shift
	ldr	r0, =0x30808000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ DDR B  Read LCDL '1' shift
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00010100
	add	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ DDR B  Write LCDL '-1' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	@ DDR B  Read LCDL '3' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00030300
	add	r1, r1, r2
	str	r1, [r0, #0x624]
	
	@ DDR C  Write LCDL '-1' shift
	ldr	r0, =0x30818000
	ldr	r1, [r0, #0x5E4]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x5E4]
	@ DDR C  Write LCDL '-1' shift
	ldr	r1, [r0, #0x624]
	ldr	r2, =0x00000001
	sub	r1, r1, r2
	str	r1, [r0, #0x624]
	

	ldr	r0, =0x30408000
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to Config mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	11b
        
	rsetl	r0, #0x484, r1, 0xBFFF0000      @ VT Compensation ON      
		
	ldr	r2, =0xFFFF0000
10:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	10b
	
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]
        
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to Access mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	11b


	ldr	r0, =0x30808000
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to Config mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	11b
        
	rsetl	r0, #0x484, r1, 0xBFFF0000      @ VT Compensation ON      
		
	ldr	r2, =0xFFFF0000
10:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	10b
	
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]
        
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to Access mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	11b	


	ldr	r0, =0x30818000
	rsetl	r0, #0x004, r1, 0x00000001	@ Change to Config mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000001
	bne	11b
        
	rsetl	r0, #0x484, r1, 0xBFFF0000      @ VT Compensation ON      
		
	ldr	r2, =0xFFFF0000
10:	ldr	r1, [r0, #0x484]
	and	r1, r1, r2
	cmp	r1, r2
	bne	10b
	
	ldr	r1, [r0, #0x404]
	ldr	r2, =0xEFFFFFFF
	and	r1, r1, r2
	str	r1, [r0, #0x404]
        
	rsetl	r0, #0x004, r1, 0x00000002	@ Change to Access mode
11:	ldr	r1, [r0, #0x008]
	and	r1, r1, #0x00000007
	cmp	r1, #0x00000003
	bne	11b	

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
	rsetl	r0, #0x008, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x00C, r1, 0x00000000	@ max timeout for p0
	rsetl	r0, #0x010, r1, 0x02010201	@ max timeout for p1 th0
	rsetl	r0, #0x014, r1, 0x02010201	@ max timeout for p1 th0
	rsetl	r0, #0x018, r1, 0x01010101	@ max timeout for p1 th1
	rsetl	r0, #0x01C, r1, 0x01010101	@ max timeout for p1 th1
	rsetl	r0, #0x020, r1, 0x01810181	@ max timeout for p1 th2
	rsetl	r0, #0x024, r1, 0x01810181	@ max timeout for p1 th2
	rsetl	r0, #0x028, r1, 0x02010201	@ max timeout for p1 th3
	rsetl	r0, #0x02C, r1, 0x02010201	@ max timeout for p1 th3


	@XMIF setting
	ldr	r0, =0x30650000
	rsetl   r0, #0x000, r1, 0x01213233
	rsetl	r0, #0x004, r1, 0x0D232220

