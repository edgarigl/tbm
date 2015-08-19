MEMORY { boot_ram (RWX) : ORIGIN = 0x80000000, LENGTH = 0x4000000 }

SECTIONS
{
	/* Simple linker script. */
	ENTRY(_start)

	.vectors :
	{
		KEEP(*(.vectors*))
	} >boot_ram

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
		. = ALIGN(32);
	} >boot_ram

	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(8);
		. = ALIGN(32);
		_edata = . ;
	} >boot_ram

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(8);
		_ebss = . ;
		/* heap.  */
		_heap_start = .;
		. = . + 0x100;
		_heap_end = .;
	} >boot_ram

	_stack = 0x85000000;
	_end = .;

	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.note.gnu.build-id) }
}
