/*
 * Bare-metal Xilinx ZynqMPSoc setup.
 * Copyright (C) 2015 Edgar E. Iglesias.
 */
#define _MINIC_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sys.h"
#include "plat.h"
#include "mmu.h"
#include "cache.h"

#define D(x)

static struct {
        struct aarch64_cache_descr caches;
        unsigned int freq;
} vm;

__attribute__ ((weak)) char _pt_start;
__attribute__ ((weak)) char _pt_end;

static struct mmu_ctx mmu;

static void plat_setup_mmu(unsigned int el)
{
	if (&_pt_start + 4 >= &_pt_end) {
		printf("WARN: No space for MMU tables.\n");
		return;
	}

	aarch64_mmu_pt_setup(&mmu, PAGEKIND_TINY_4K, 1,
				40, 40,
				&_pt_start,
				&_pt_end);

	aarch64_mmu_map(&mmu, 0, 0, MAPSIZE_1G, PROT_RWE, MAIR_IDX_DEVICE, MAP_SECURE);

	/* Xen RAM at 0x40000000  */
	aarch64_mmu_map(&mmu, 1ULL << 30, 1ULL << 30, MAPSIZE_1G, PROT_RWE,
			MAIR_IDX_MEM, MAP_SECURE);
	aarch64_mmu_map(&mmu, 2ULL << 30, 2ULL << 30, MAPSIZE_1G, PROT_RW,
			MAIR_IDX_MEM, MAP_SECURE);
	aarch64_mmu_map(&mmu, 3ULL << 30, 2ULL << 30, MAPSIZE_1G, PROT_RW,
			MAIR_IDX_MEM, MAP_SECURE);


	/* Invalidate caches before enabling them.  */
	a64_icache_invalidate(&vm.caches);
	a64_dcache_invalidate(&vm.caches, false);
	aarch64_enable_caches(el, true, true);
	printf("EL%d I and D caches enabled\n", el);

	printf("RDO: enable MMU\n");
	aarch64_mmu_setup(&mmu, el, true);
	printf("RDO: Still here? EL%d S1 MMU is on\n", el);
}

void plat_cache_invalidate(void)
{
	a64_icache_invalidate(&vm.caches);
	a64_dcache_invalidate(&vm.caches, false);
	ibarrier();
	mb();
	/* Clean state.  */
}

void plat_cache_flush(void)
{
	a64_icache_invalidate(&vm.caches);
	a64_dcache_invalidate(&vm.caches, true);
	ibarrier();
	mb();

	/* Clean state.  */
}

void plat_init_a64(void)
{
	const unsigned int current_el = aarch64_current_el();

	printf("%s\n", __func__);
	plat_setup_mmu(current_el);
}

void plat_init(void)
{
	a64_cache_discover(&vm.caches);
	plat_init_a64();
}
