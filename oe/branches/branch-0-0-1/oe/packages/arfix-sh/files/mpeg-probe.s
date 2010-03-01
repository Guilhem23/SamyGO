/* mpeg-probe.s

 mpeg-probe patches for autoselecting video aspect ratio
 for Samsung TV with T-RBYDEUC firmware 
*/
        .little

	.extern mpegAspectRatio
	.extern MpegProbeSetSize

        .section patch4ParserReset,"ax",@progbits
ParserResetPatched:
	.global ParserResetPatched
	mov.l	.L1,r0
	mov	#0,r1
	bra	.L4
	mov.l	r1,@r0
.L1:
	.long	mpegAspectRatio
.L4:

        .section .patchtext,"ax",@progbits

MpegProbeSetSizePatched:
	.global MpegProbeSetSizePatched
	mov.b	@r8,r0
	mov.l	.L3,r1
	mov.b	r0,@r1

	mov.l	.L2,r1
	jmp	@r1
	nop
	
.L2:
	.long	MpegProbeSetSize
.L3:
	.long	mpegAspectRatio

