INCLUDE "configs/pele-mem.ldh"

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
	} >ddr

	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(4);
		_edata = . ;
	} >ddr

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(4);
		_ebss = . ;
	} >ddr

	/* heap.  */
	_heap_start = .;
	. = . + 0x8000;
	_heap_end = .;

	. = . + 0x2000;
	_stack = .;

	_end = .;

	/DISCARD/ : { *(.eh_frame) }
}
