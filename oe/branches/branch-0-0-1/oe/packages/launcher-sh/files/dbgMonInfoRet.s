/* dbgMonInfoRet.s

 patch for Samsung TV with T-RBYDEUC firmware 
*/
        .little

	.extern injlib

        .section .patchtextDbgMonInfoRet1,"ax",@progbits
/* 0x69FDB8 */
L1:
	cmp/eq  #1, r0
        bf	L1 + 0x69FE4C - 0x69FDB8
	

        .section .patchtextDbgMonInfoRet2,"ax",@progbits
/* 0x69FE4C */
L2:
	exts.b	r0, r0
	cmp/eq	#0x99, r0
	bf	L2 - 0x69FE4C + 0x69FD84
	add	#4, r14
	bra	L2 - 0x69FE4C + 0x69FB00
/*
	mov	r14, r15
	lds.l	@r15+, pr
	mov.l	@r15+, r14
	mov.l	@r15+, r13
	mov.l	@r15+, r12
	mov.l	@r15+, r11
	mov.l	@r15+, r10
	mov.l	@r15+, r9
	mov.l	@r15+, r8
	rts
*/
	nop


        .section .patchtextStartUPnP,"ax",@progbits
	mov     #1, r0
/* 0x11B1156 */
	mov	r0, r8
	mov.l	L3, r0
	jsr	@r0
	nop
	mov	r8, r0
	mov     r14, r15
	lds.l   @r15+, pr
	mov.l   @r15+, r14
	mov.l   @r15+, r8
	rts
	nop
L3:
	.long	InjLibStart
