INCLUDE "configs/zynqmp-mem.ldh"

MEMORY { tcm_rpu (RX) : ORIGIN = 0x0, LENGTH = 0x10000 }
MEMORY { ddr_rpu (RX) : ORIGIN = 0x3ed00000, LENGTH = 0x800000 }

SECTIONS
{
	/* Simple linker script. */
	ENTRY(_start)

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
		KEEP(*(.initcall*))
		__initcalls_end = .;
		. = ALIGN(4);
		__testcalls_start = .;
		KEEP(*(.testcall*))
		__testcalls_end = .;
		. = ALIGN(4);
		_etext = . ;
	} >tcm_rpu

	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = . ;
	} >tcm_rpu

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_ebss = . ;
	} >tcm_rpu

	/* heap.  */
	_heap_start = .;
	. = . + 0xf000;
	_heap_end = .;

	. = . + 0x8000;
	_stack = .;

	_end = .;

	/DISCARD/ : { *(.note.gnu.build-id) }
	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.interp .dynamic) *(.dynsym .dynstr .hash .gnu.hash) }
}
