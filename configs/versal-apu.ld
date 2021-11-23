OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
TARGET(binary)

INCLUDE "configs/versal-mem.ldh"

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
	} >ocm

	/* Can be reused after dtb parsing.  */
	_dtb = .;
	.dtb : {
		KEEP(./versal-apu.dtb.o)
	} >ocm
	_dtb_end = .;

	.data :
	{
		_sdata = . ;
		*(.data)
		*(.data*)
		. = ALIGN(8);
		_edata = . ;
	} >ocm

	.bss :
	{
		_sbss = . ;
		*(.bss)
		*(.bss*)
		. = ALIGN(8);
		_ebss = . ;
	} >ocm

	/* heaps.  */
	. = ALIGN(32);
	_heap_start = .;
	_heap_ocm_start = .;
	. = . + 0x8000;
	_heap_ocm_end = .;
	_heap_end = .;
	/* DDR heap.  */
	_heap_ddr_start = 0x1000; _heap_ddr_end = 0x1000000;
	_pt_start = 0x1000000; _pt_end = 0x1200000;
	_pt_s2_start = 0x1200000; _pt_s2_end = 0x1400000;

	/* Stack.  */
	. = ALIGN(16);
	. = . + 0x2000;
	_stack = .;

	_end = .;

	/DISCARD/ : { *(.note.gnu.build-id) }
	/DISCARD/ : { *(.eh_frame) }
	/DISCARD/ : { *(.interp .dynamic) *(.dynsym .dynstr .hash .gnu.hash) }
}
