MEMORY { ddr (RWX) : ORIGIN = 0x80080000, LENGTH = 0x100000 }

SECTIONS
{
	/* Simple linker script. */
	ENTRY(_start)

	.vectors :
	{
		KEEP(*(.vectors*))
	} >ddr

	.text :
	{
		_stext = . ;
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)
		*(.fixup)
		*(.got)
		. = ALIGN(8);
		__initcalls_start = .;
		KEEP(*(.initcall*))
		__initcalls_end = .;
		. = ALIGN(8);
		__testcalls_start = .;
		KEEP(*(.testcall*))
		__testcalls_end = .;
		. = ALIGN(8);
		_etext = . ;
	} >ddr

	.data :
	{
		_sdata = . ;
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

	. = ALIGN(4096);
	_pt_start = .;
	. = . + 0x1200000;
	_pt_end = .;
	_pt_s2_start = .;
	. = . + 0x1200000;
	_pt_s2_end = .;

	/* Stack.  */
	. = . + 0x8000;
	_stack = .;

	_end = .;

	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.note.gnu.build-id) }
}
