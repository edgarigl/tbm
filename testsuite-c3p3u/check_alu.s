
	.global	check_alu_add
	.type	check_alu_add,@function
check_alu_add:
	subi	$sp, $sp, 4
	sw	$lr, $sp, 0

	bli	$lr, puts
	movi	$r0, check_alu_add_str
	nop

	movi	$r0, 0
	addi	$r0, $r0, 1
	bnei	$r0, 1, fail
	addi	$r0, $r0, 1
	bnei	$r0, 2, fail
	addi	$r0, $r0, 10

	movi	$r1, 12
	bne	$r0, $r1, fail
	nop


	movi	$r0, -1
	movi	$r1, 1
	add	$r0, $r0, $r1
	bnei	$r0, 0, failed
	nop

	movi	$r0, 0
	movi	$r1, -1
	add	$r0, $r0, $r1
	bnei	$r0, -1, failed
	nop

	bli	$lr, passed
	nop
	b	done
	nop
fail:	bli	$lr, failed
	nop
done:
	lw	$lr, $sp, 0
	j	$lr
	addi	$sp, $sp, 4
	.size	check_alu_add,. - check_alu_add

	.section	".rodata"
check_alu_add_str:	.asciz	"check ALU add "
