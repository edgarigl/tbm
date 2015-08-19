#include "aarch64-asm.h"

#	.section .vectors, "ax", @progbits
	.balign 32
	.global vectors
vectors:
	/* HW reset.  */
	ldr	x16, =_start
	br	x16

	/* text offset.  */
	.dword	0
	/* image size.  */
	.dword	0
	/* flags.  */
	.dword	0
	/* RES0  */
	.dword	0
	.dword	0
	.dword	0

	/* magic  */
	.dword	0x644d5241
	/* RES0  */
	.dword	0


	.org	0x200
	b	excp_decode

	.org	0x300
	store_all_regs
	mov	x0, sp
	isb
	ldr	x17, =aarch64_fiq
	blr	x17
	b	ret_from_excp

	.org	0x380
	b	excp_decode

	.org	0x400
	b	excp_decode

	.org	0x580
excp_decode:
	store_all_regs
	mov	x0, sp
	isb
	ldr	x17, =aarch64_excp_decode
	blr	x17

ret_from_excp:
	load_all_regs
	isb
	eret

	.text
	/* ELF entry point.  */
	.global _start
_start:
	mrs     x2, mpidr_el1
	and	x2, x2, 0xff
	cbz	x2, 2f
1:
	# Secondaries are not expected to enter here.
	wfi
	b	1b

2:
	mov	x1, #1
	msr	spsel, x1
	isb
	ldr	x1, =_stack
	mov	sp, x1

	bl	__arch_early_init

	ldr	x0, =_sbss
	mov	x1, xzr
	ldr	x2, =_ebss
	sub	x2, x2, x0
	bl	memset

	ldr	x0, =_heap_start
	ldr	x1, =_heap_end
	sub	x1, x1, x0
	bl	alloc_init
	bl	__initcalls_exec
	bl	main
	nop
	.size _start, . - _start

	.global _secondary_entry_head
_secondary_entry_head:
	ldr	x0, =dummy_stack_top
	mrs     x2, mpidr_el1
	and	x2, x2, #0x3
	add	x2, x2, #1
	lsl	x2, x2, #10
#	add	x0, x0, x2

	mov	sp, x0

	ldr	x0, =vectors
	msr	vbar_el3, x0
	msr	vbar_el2, x0
	msr	vbar_el1, x0
	isb

	mov	x0, #0x400
	msr	scr_el3, x0
	mov	x0, #0
	msr	sctlr_el3, x0
	msr	tcr_el3, x0
	isb

	bl	secondary_entry

	.global	tbm_fdt
	.section .bss
tbm_fdt:
	.word	0

dummy_stack_end:
	.balign	32
	.skip	4 * 1024
dummy_stack_top:

