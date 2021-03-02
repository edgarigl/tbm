#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "regops.h"

#include "testsuite-generic/memtest.h"
#include "drivers/versal/versal-lpd-memmap.dtsh"
#include "drivers/versal/reg-xram_ctrl.h"

#define XRAM_SIZE_64K	0
#define XRAM_SIZE_128K	1
#define XRAM_SIZE_256K	2
#define XRAM_SIZE_512K	3
#define XRAM_SIZE_1M	4

static void check_xram(phys_addr_t base, int size_code)
{
	uint32_t r;

	printf("%s: base=%llx\n", __func__, (uint64_t) base);
	r = readl(base + R_XRAM_CTRL_XRAM_IMP);
	assert(r == size_code);
}

void check_xrams(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		check_xram(MM_OCM2 + i * 0x10000, XRAM_SIZE_1M);
	}
	tbm_memtest(MM_OCM2_MEM, 4 * 1024 * 1024);
}
