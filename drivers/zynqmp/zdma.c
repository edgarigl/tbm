/*
 * Small zDMA driver
 *
 * Copyright (C) 2014 Xilinx Inc
 * Written by Edgar E. Iglesias
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <byteswap.h>

#include "sys.h"
#include "plat.h"

#include "zdma.h"

#define D(x)

/* Compute a base address for a given block and channel.  */
unsigned char *zdma_base(char *base, unsigned int ch)
{
	unsigned int ch_offset = ch * 0x10000;
	unsigned char *ch_base = (unsigned char *) base + ch_offset;

	return ch_base;
}

void zdma_reset(char *base, unsigned int ch)
{
#if 0
	unsigned char *ch_base = zdma_base(base, ch);

	writel(ch_base + ZDMA_ZDMA_SOFT, 1);
	/* FIXME: Do we need to wait a certain amount of time?  */
	writel(ch_base + ZDMA_ZDMA_SOFT, 0);
#endif
}

/* Wait for a given state.  */
int zdma_wait_for(char *base, unsigned int ch, int wstate)
{
	unsigned char *ch_base = zdma_base(base, ch);
	int status;

	do {
		status = readl(ch_base + ZDMA_ZDMA_CH_STATUS);
	} while (status != wstate);

	return status;
}

/* Allocate a set of continous descriptors with proper alignment.  */
struct zdma_descr *zdma_descr_alloc(struct alloc_ctx *area,
				unsigned int nr, char **mem)
{
	struct zdma_descr *dsc;
	uintptr_t ap;

	*mem = _malloc_ctx(area, sizeof *dsc * nr + sizeof *dsc - 1);
	ap = (uintptr_t) *mem;
	ap = (ap + sizeof *dsc - 1) & ~(sizeof *dsc - 1);
	return (void *) ap;
}

void zdma_descr_init(struct zdma_descr *d, void *addr,
			    unsigned int size,
			    unsigned int cmd,
			    bool intr, bool ptype, bool coherent)
{
	d->addr = (uintptr_t) addr;
	d->size = size;
	d->attr = cmd << ZDMA_ZDMA_CH_SRC_DSCR_WORD3_CMD_SHIFT;
	d->attr |= intr << ZDMA_ZDMA_CH_SRC_DSCR_WORD3_INTR_SHIFT;
	d->attr |= ptype << ZDMA_ZDMA_CH_SRC_DSCR_WORD3_TYPE_SHIFT;
	d->attr |= coherent << ZDMA_ZDMA_CH_SRC_DSCR_WORD3_COHRNT_SHIFT;
}

/* Write a 64bit value into two contigous regs.  */
void zdma_write64(void *base, uint64_t val64)
{
	uint32_t *base32 = base;

	writel(base32, val64);
	writel(base32 + 1, val64 >> 32);
}
