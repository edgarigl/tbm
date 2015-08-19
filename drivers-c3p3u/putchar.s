	.global	uart_putchar
	.type	uart_putchar,@function
uart_putchar:
	movi	$r2, 0x5002
	lsli	$r2, $r2, 16
	; wait for the port to become ready.
1:	lw	$r1, $r2, 0
	andi	$r1, $r1, 1
	beqi	$r1, 0, 1b
	nop

	sw	$r0, $r2, 0
	nop

	movi	$r1, '\n'
	beq	$r0, $r1, 1b
	movi	$r0, '\r'

	j	$lr
	nop
	.size	uart_putchar, . - uart_putchar
