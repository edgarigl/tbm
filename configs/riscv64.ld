SECTIONS
{
	/* Simple linker script. */
	ENTRY(_start)

	.text :
	{
		_stext = . ;
		*(.text)
		*(.text*)
		. = ALIGN(8);
		*(.rodata)
		*(.rodata*)
		. = ALIGN(8);
		*(.fixup)
		. = ALIGN(8);
		*(.got)
		. = ALIGN(4);
		__initcalls_start = .;
		KEEP(*(.initcall*))
		__initcalls_end = .;
		. = ALIGN(4);
		__testcalls_start = .;
		KEEP(*(.testcall*))
		__testcalls_end = .;
		. = ALIGN(8);
		_etext = . ;
	} >ddr

	.data :
	{
		_sdata = . ;
		*(.sdata)
		*(.sdata*)
		. = ALIGN(8);
		*(.data)
		*(.data*)
		. = ALIGN(8);
		_edata = . ;
	} >ddr

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(8);
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
