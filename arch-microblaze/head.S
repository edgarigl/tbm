# Used for validating QEMU.
# This code is not guaranteed to work on real HW, needs careful testing
# in case it gets reused.
#
# Written by Edgar E. Iglesias
#
# The build scripts provide two bootable images.
# 1. An ELF file to be bootloaded (for QEMU)
# 2. A binary file image to be put in ROM (for QEMU and real HW)
#
# The ELF file contains code and readonly data in ROM addresses and
# has the initialized .data section with contents at private RAM.
# QEMU will load everything into the proper place and boot.
#
# Om real HW, RAM content needs to be loaded by the ROM code itself.
# So we build the binary image to append a .data initialization template
# right after the .text and .rodata. Then at boot, we copy the ROM:ed .data
# template into its correct location in RAM (_sdata).
#
# The copying of ROM .data template into RAM should not be done
# for ELF loads, because the ROM template wont be there, it will
# results in trashed .data. We therefor set the ELF entry point
# to _start and the HW reset point to __bare_start and can support
# both kinds of boots.
#

#include "arch-irq.h"

	# Reset and exceptions/interrupt vectors
	.global __reset_vector
__reset_vector:
	# 0x00  - Reset
	bri	__bare_start
	.balign 8
	# 0x08  - Break 8
	bri	__break_8_handler
	.balign 8
	# 0x10  - IRQ
	bri	__interrupt_handler
	.balign 8
	# 0x18  - IRQ
	bri	__break_18_handler
	.balign 8
	# 0x20  - HW Exceptions
	bri	__hw_exception_handler
	.balign 8
	.size __reset_vector, . - __reset_vector

	.global	__bare_start
__bare_start:
	# Setup stack to top of private RAM.
	addik	r1, r0, _sdata
	mts     rslr,   r1
	addik	r1, r0, _sdata + 0x8000
	mts     rshr,   r1

	# Copy the .data template from ROM to RAM. The template is
	# located at _etext. Copy it into _sdata. Lenght is (_edata - s_data)
	#
	# memcpy(_sdata, _etext, _edata - _sdata);
	addik	r5, r0, _sdata
	addik	r6, r0, _etext
	addik	r7, r0, _edata
	brlid	r15, memcpy
	rsub	r7, r5, r7	# r7 = r7 - r5 (edata - sdata)
	.size __bare_start, . - __bare_start

	# ELF entry point.
	.global	_start
_start:
	# Reinit the stack for ELF-loads.
	addik	r1, r0, _sdata
	mts     rslr,   r1
	addik	r1, r0, _sdata + 0x8000
	mts     rshr,   r1

	# Zero the .bss area. memset(_sbss, 0, _ebss - _sbss)
	addik	r5, r0, _sbss
	addik	r7, r0, _ebss
	rsub	r7, r5, r7
	brlid	r15, memset
	addik	r6, r0, 0

	# Initialize malloc. 8K of RAM, starting at the end of .bss.
	addik	r5, r0, _heap_start
	addik	r6, r0, _heap_end
	brlid	r15, alloc_init
	rsub    r6, r5, r6

	brlid	r15, __initcalls_exec
	nop

	# Jump into C code.
	brlid	r15, main
	nop

1:
	bri	1b
	nop
	.size _start, . - _start


	# Macros to save and restore the CPU state.
	.macro	save_reg n
		swi	r\n, r1, \n * 4
	.endm
	.macro	restore_reg n
		lwi	r\n, r1, \n * 4
	.endm

	.macro	operate_reg op, n
		\op	r\n, r1, \n * 4
	.endm

	# Can we evaluate arithmetic expressions in gnu asm preprocessor
	# and autogenerate this code?
	.macro	cpustate_operate_regs op
		operate_reg \op, 2
		operate_reg \op, 3
		operate_reg \op, 4
		operate_reg \op, 5
		operate_reg \op, 6
		operate_reg \op, 7
		operate_reg \op, 8
		operate_reg \op, 9
		operate_reg \op, 10
		operate_reg \op, 11
		operate_reg \op, 12
		operate_reg \op, 13
		operate_reg \op, 14
		operate_reg \op, 15
		operate_reg \op, 16
		operate_reg \op, 17
		operate_reg \op, 18
		operate_reg \op, 19
		operate_reg \op, 20
		operate_reg \op, 21
		operate_reg \op, 22
		operate_reg \op, 23
		operate_reg \op, 24
		operate_reg \op, 25
		operate_reg \op, 26
		operate_reg \op, 27
		operate_reg \op, 28
		operate_reg \op, 29
		operate_reg \op, 30
		operate_reg \op, 31
	.endm
	.macro	cpustate_save
		cpustate_operate_regs swi
		mfs	r11, rmsr
		swi	r11, r1, CPUSTATE_MSR_OFFSET
	.endm
	.macro	cpustate_restore
		lwi	r11, r1, CPUSTATE_MSR_OFFSET
		mts	rmsr, r11
		cpustate_operate_regs lwi
	.endm
	.macro	cpustate_set_pc r
		swi	\r, r1, CPUSTATE_PC_OFFSET
	.endm

#
# Entry points for the various kinds of exceptions.
# For the moment we just do the minimum so that
# the testsuite can track the various events.
#
# Nothing useful.
#
	.global __break_8_handler
__break_8_handler:
	addik	r1, r1, -4
	swi	r2, r1, 0

	lwi	r2, r0, _nr_brk8
	addik	r2, r2, 1
	swi	r2, r0, _nr_brk8

	lwi	r2, r1, 0
	rtbd	r15, 4
	addik	r1, r1, 4
	.size __break_8_handler, . - __break_8_handler

#
# Interrupts. These propagate to C code.
#
	.global __interrupt_handler
__interrupt_handler:
	addik	r1, r1, -CPUSTATE_SIZE
	cpustate_save
	cpustate_set_pc r14

	# step into generic C irq code.
	brlid   r15, arch_interrupt_handler
	addk	r5, r1, r1

	cpustate_restore
	rtid	r14, 0
	addik	r1, r1, CPUSTATE_SIZE
	.size __interrupt_handler, . - __interrupt_handler

	.global __break_18_handler
__break_18_handler:
	addik	r1, r1, -4
	swi	r2, r1, 0

	lwi	r2, r0, _nr_brk18
	addik	r2, r2, 1
	swi	r2, r0, _nr_brk18

	lwi	r2, r1, 0
	rtbd	r16, 4
	addik	r1, r1, 4
	.size __break_18_handler, . - __break_18_handler

	.global __hw_exception_handler
__hw_exception_handler:
	# This version assumes we hit the testcase
	# that intentionally steps into an illegal
	# instruction. We just step over an insn here.
	#
	addik	r1, r1, -4
	swi	r2, r1, 0

	lwi	r2, r0, _nr_hw_excp
	addik	r2, r2, 1
	swi	r2, r0, _nr_hw_excp

	lwi	r2, r1, 0
	# + 4 to step over the bad insn
	rted	r17, 4
	addik	r1, r1, 4
	.size __hw_exception_handler, . - __hw_exception_handler

# Stats for the testsuite to keep track of the various exceptions.
	.bss
	.global _nr_brk8
	.global _nr_brk18
	.global _nr_hw_excp
_nr_brk8:
	.word 0
_nr_brk18:
	.word 0
_nr_hw_excp:
	.word 0
