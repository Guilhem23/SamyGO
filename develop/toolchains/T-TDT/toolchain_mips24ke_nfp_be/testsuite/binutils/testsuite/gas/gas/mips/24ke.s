#The assembler should warn when a float instruction follows a branch-likely,
#but not for other types of instruction
foo:
	.set noreorder
	.set nomacro

	# good
	beql	$0, $2, $L1
	add	$0, $2, $3

	# bad
	beql	$2, $3, $L1
	add.s	$f0, $f2, $f3

$L1:
	nop
