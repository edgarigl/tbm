#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys.h"

void app_run(void)
{
	void (*start_kernel)(void *fdt, int a0, int a1, int a2) = (void *) (uintptr_t) 0x80080000;
	extern char _binary___a64_linux_dtb_start;
	extern char _binary___a64_linux_dtb_end;
	extern char _binary___a64_linux_img_start;
	extern char _binary___a64_linux_img_end;
	char *kernel_end = (void *) start_kernel;

	printf("Test-Baremetal Aarch64 Bootloader\n");
	kernel_end += &_binary___a64_linux_img_end - &_binary___a64_linux_img_start;

	aarch64_drop_el(EL2, SPXH);
	printf("move dtb into place\n");
	memmove((void *)(uintptr_t) 0x84000000, &_binary___a64_linux_dtb_start,
		&_binary___a64_linux_dtb_end - &_binary___a64_linux_dtb_start);
#if 1
	printf("move image into place to=%p from=%p size=%ld\n",
		start_kernel, &_binary___a64_linux_img_start,
		&_binary___a64_linux_img_end - &_binary___a64_linux_img_start + 1);
#endif
	memmove(start_kernel, &_binary___a64_linux_img_start,
		&_binary___a64_linux_img_end - &_binary___a64_linux_img_start);

	hexdump("dtb", (void *) (uintptr_t) 0x84000000, 64);
	hexdump("kernel", (void *) (uintptr_t) 0x80080000, 64);
	start_kernel((void *) (uintptr_t) 0x84000000, 0, 0, 0);
}
