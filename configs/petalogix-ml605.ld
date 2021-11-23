/*
 * Copyright (C) Xilinx Inc
 * Written by Edgar E. Iglesias
 */
MEMORY { ddr (RWX) : ORIGIN = 0x50000000, LENGTH = 0x40000 }
MEMORY { bram2 (RWX) : ORIGIN = 0x00040000, LENGTH = 0x40000 }

SECTIONS
{
	/* Setup the ELF entry point. This will not necessarily match
	   the HW entry point.  */
	ENTRY(_start)

	/* Stuff that links to ROM.  */
	.text :
	{
		_stext = . ;
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)
		*(.fixup)
		*(.got)
		. = ALIGN(4);
		__initcalls_start = .;
		KEEP(*(.initcall))
		__initcalls_end = .;
		. = ALIGN(4);
		__testcalls_start = .;
		KEEP(*(.testcall))
		__testcalls_end = .;
		. = ALIGN(4);
		_etext = . ;
	} >ddr

	/* Initialized data that links to RAM.  */
	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = . ;
	} >ddr

	/* Uninitialized data that links to RAM.  */
	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_ebss = . ;
	} >ddr

	_heap_start = .;
	. += 0x2000;
	_heap_end = .;

	. = ALIGN(16);
	_stack_end = .;
	. += 0x2000;
	_stack = .;
	_end = .;

	/DISCARD/ : { *(.note.gnu.build-id) }
	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.interp .dynamic) *(.dynsym .dynstr .hash .gnu.hash) }
}
