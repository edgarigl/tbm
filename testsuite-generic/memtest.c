/*
 * Simple memtest routines.
 *
 * Copyright (C) 2021 Xilinx Inc.
 * Written by Edgar E. Iglesias <edgar.iglesias@xilinx.com>
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "sys.h"

/*
 * Run memtests. This runs a destructive test-pattern.
 */
static void test_mem_pattern(phys_addr_t addr, uint32_t v)
{
	uint32_t r;
	writel(addr, v);
	r = readl(addr);
	assert(r == v);
}

void tbm_memtest(phys_addr_t addr, uint64_t len)
{
	uint64_t i;
	uint32_t v;
	int bit;

	printf("%s: addr=%lx len=%lld\n", __func__, addr, len);
	for (i = 0; i < len; i += 4) {
		v = 0xFFFFFFFFU;
		test_mem_pattern(addr + i, v);

		for (bit = 0; bit < 32; bit++) {
			// Turn off one bit at a time.
			v ^= (1 << bit);
			writel(addr + i, v);
			v = readl(addr + i);
			assert(v == v);
		}
	}
}
