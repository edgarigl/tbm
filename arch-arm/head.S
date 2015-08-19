reset_entry:
	b	reset
	b	_invalid_exception
	b	_invalid_exception
	b	_invalid_exception
	b	_invalid_exception
	b	_invalid_exception
	b	_invalid_exception
	/* IRQ.  */
	b	_invalid_exception
	/* FIQ.  */
	b	_invalid_exception

	/* HW reset.  */
reset:
	mov	r0, #0x0
	b	1f

	/* ELF entry point.  */
	.global _start
_start:
	mov	r0, #0x1
1:
	/* Start booting.  */
	mrc	p15, #0, r1, c0, c0, 5
	/* Allow index 4 as First RPU.  */
	ands	r1, r1, #0x3
	beq	2f
	# Secondary cores, spin. At the moment we never use
	# them so the spin for ever.
1:	wfi
	b	1b
2:

	ldr	sp, =_stack

	# Enable Advanced SIMD
	mrc	p15, 0, r1, c1, c0, 2
	orr	r1, r1, #(0x3 << 20)
	mcr	p15, 0, r1, c1, c0, 2
	isb
#if 0
	fmrx	r1, FPEXC
	orr	r1, r1, #(1 << 30)
	fmxr	FPEXC, r1
#endif

	# If we didnt get an ELF image loaded, we need
	# to setup the data area.
	cmp	r0, #0
	bne	1f
	ldr	r0, =_sdata
	ldr	r1, =_etext
	ldr	r2, =_edata
	sub	r2, r2, r0
	bl	memcpy
1:
#	Useful to trigger udef for testing.
#	mrc	15, 1, r0, cr9, cr0, 2

	bl	__arch_early_init

	ldr	r0, =_sbss
	ldr	r1, =0
	ldr	r2, =_ebss
	sub	r2, r2, r0
	bl	memset

	ldr	r0, =_heap_start
	ldr	r1, =_heap_end
	sub	r1, r1, r0
	bl	alloc_init
	nop

	bl	__initcalls_exec
	nop
	bl	main

	.size _start, . - _start


	/* We dont do exceptions yet.  */
_invalid_exception:
	b	_invalid_exception
