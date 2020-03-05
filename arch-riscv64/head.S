# Used for validating QEMU.
# This code is not guaranteed to work on real HW, needs careful testing
# in case it gets reused.
#
# Written by Edgar E. Iglesias
#
	# Reset and exceptions/interrupt vectors
	.global __reset_vector
__reset_vector:
1:
	j	1b
	.size __reset_vector, . - __reset_vector

	.global	__bare_start
	.balign 4
__bare_start:
1:
	j 1b
	# Setup stack to top of private RAM.

	# Copy the .data template from ROM to RAM. The template is
	# located at _etext. Copy it into _sdata. Lenght is (_edata - s_data)
	#
	# memcpy(_sdata, _etext, _edata - _sdata);
	.size __bare_start, . - __bare_start

	# ELF entry point.
	.global	_start
	.balign 4
_start:
	# Reinit the stack for ELF-loads.
	la sp, _stack

	# Zero the .bss area. memset(_sbss, 0, _ebss - _sbss)
	la a0, _sbss
	la a1, 0
	la a2, _ebss
	subw a2, a2, a0
	call memset

	# Initialize malloc. 8K of RAM, starting at the end of .bss.

	# Jump into C code.
#1:
#	j	1b
	call main

	.size _start, . - _start
