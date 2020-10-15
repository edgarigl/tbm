/*
 * TBM RV32 start-up code.
 * Copyright (C) 2020 Edgar E. Iglesias
 * Written by Edgar E. Iglesias <edgar.iglesias@gmail.com>
 */

#define MAGIC_DATA_VAL 0x1337

	/* Reset and exceptions/interrupt vectors */
	.global __reset_vector
__reset_vector:
	j	__bare_start
	.size __reset_vector, . - __reset_vector

	.global	__bare_start
	.balign 4
__bare_start:
	la sp, _stack

	/*
	 * See if data_val got initialized.
	 * Assume this was a miss-loaded ELF (loaded but with wrong start-addr).
	 * If _data_val is correctly initialized. Otherwise, we assume this
	 * was loaded as a blob.
	 */
	la a0, _data_val
	lw a1, (a0)
	li a0, MAGIC_DATA_VAL
	beq a0, a1, _start

	/*
	 * Copy the .data template from ROM to RAM. The template is
	 * located at _etext. Copy it into _sdata. Lenght is (_edata - s_data)
	 * memcpy(_sdata, _etext, _edata - _sdata);
	 */
	la a0, _sdata
	la a1, _etext
	la a2, _edata
	sub a2, a2, a1
	call memmove

	la a0, _data_val
	lw a1, (a0)
	li a0, MAGIC_DATA_VAL
1:
	bne a0, a1, 1b

	.size __bare_start, . - __bare_start

	/* ELF entry point.  */
	.global	_start
	.balign 4
_start:
	/* Reinit the stack for ELF-loads.  */
	la sp, _stack

	/* Zero the .bss area. memset(_sbss, 0, _ebss - _sbss)  */
	la a0, _sbss
	li a1, 0
	la a2, _ebss
	sub a2, a2, a0
	call memset

	/* Initialize malloc. 8K of RAM, starting at the end of .bss. */
	la a0, _heap_start
	la a1, _heap_end
	sub a1, a1, a0
	call alloc_init
	call __initcalls_exec

	/* Jump into C code. */
	call main
1:
	wfi
	j 1b
	.size _start, . - _start

	.data
_data_val:
	.word	MAGIC_DATA_VAL
