/*
 * Bare-metal Xilinx Versal PMC setup.
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
#ifdef HAVE_FDT
#include "devtree.h"
#endif

#define D(x)

extern char _dtb;
struct alloc_areas plat_mem;
static struct alloc_ctx ddr_ctx;
static struct alloc_ctx ocm_ctx;

extern char _heap_start;
__attribute__((weak)) char _heap_ddr_start;
__attribute__((weak)) char _heap_ddr_end;
__attribute__((weak)) char _heap_ocm_start;
__attribute__((weak)) char _heap_ocm_end;

static struct alloc_ctx * plat_memheap(const char *name,
					struct alloc_ctx *new,
					char *start, char *end)
{
	bool def;
	uintptr_t size_K;

	/* To small.  */
	if (end <= start + 32)
		return NULL;

	def = &_heap_start == start;
	size_K = end - start;
	size_K >>= 10;
	printf("%s: %s (%s) %p - %p %ldKB\n", __func__, name, def ? "def" : "new", start, end, size_K);
	/* This is the default HEAP.  */
	if (def) {
		return _alloc_get_default_ctx();
	}

	_alloc_ctx_init(new, start, end - start);
	return new;
}

void plat_init(void)
{
	printf("%s\n", __func__);

#ifdef HAVE_FDT
	printf("\nFDT: scan\n");
	devtree_setup(&_dtb);
	printf("FDT: done\n\n");
#endif

	plat_mem.ocm = plat_memheap("OCM", &ocm_ctx, &_heap_ocm_start, &_heap_ocm_end);
	plat_mem.ddr = plat_memheap("DDR", &ddr_ctx, &_heap_ddr_start, &_heap_ddr_end);

	putchar('\n');
}
