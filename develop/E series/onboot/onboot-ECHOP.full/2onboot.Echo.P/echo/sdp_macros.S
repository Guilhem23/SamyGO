#ifndef _SDP_MACROS_S_
#define _SDP_MACROS_S_
/*----------------------------------------------------------------------
 * macros for setting 32bit register
 *---------------------------------------------------------------------*/
	/* rsetl : str with literal value loaded */	
	.macro rsetl, base, offset, rd, lit
	ldr	\rd, =\lit
	str	\rd, [\base, \offset]
	.endm

	/* rseteql : str with literal value loaded */	
	.macro rseteql, base, offset, rd, lit
	ldreq	\rd, =\lit
	streq	\rd, [\base, \offset]
	.endm

	/* rsetnel : str with literal value loaded */	
	.macro rsetnel, base, offset, rd, lit
	ldrne	\rd, =\lit
	strne	\rd, [\base, \offset]
	.endm

	/* rsetr : str with register directly */
	.macro rsetr, base, offset, rn
	str	\rn, [\base, \offset]
	.endm

	/* dummy loop for (2*val+1) times > */
	.macro	rdelay, rd, val
	mov	\rd, \val
	1:
	subs	\rd, \rd, #1
	bgt	1b
	.endm

#endif
