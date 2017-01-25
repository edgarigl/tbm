OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
TARGET(binary)

MEMORY { ram (RX) : ORIGIN = 0x40000000, LENGTH = 0x10000000 }

SECTIONS
{
	/* Simple linker script. */
	ENTRY(_start)

	.text :
	{
		_stext = . ;
		*(.vectors*)
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)
		*(.fixup)
		*(.got)
		. = ALIGN(8);
		__fdt_probe_start = .;
		KEEP(*(.fdt_probe*))
		__fdt_probe_end = .;
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
	} >ram

	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(8);
		_edata = . ;
	} >ram

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(8);
		_ebss = . ;
	} >ram

	/* heaps.  */
	. = ALIGN(32);
	_heap_start = .;
	_heap_ocm_start = .;
	. = . + 0x8000;
	_heap_ocm_end = .;
	_heap_end = .;
	/* DDR heap.  */
	. = ALIGN(0x10000);
	_pt_start = .;
	. = . + 0x200000;
	_pt_end = .;

	/* Stack.  */
	. = ALIGN(16);
	. = . + 0x2000;
	_stack = .;

	_end = .;

	/DISCARD/ : { *(.note.gnu.build-id) }
	/DISCARD/ : { *(.eh_frame) }
}
