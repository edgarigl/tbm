/*
 * Small zDMA driver
 *
 * Copyright (C) 2014 Xilinx Inc
 * Written by Edgar E. Iglesias
 */
#define _MINIC_SOURCE

#include <stdint.h>
#include <stdbool.h>

#include "reg-adma.h"
#include "reg-gdma.h"

#define D(x)

enum {
	ZDMA_ATTR_CMD_HALT = 1,
	ZDMA_ATTR_CMD_STOP = 2,
};

enum {
	ZDMA_ATTR_COHERENT = 1 << 0,
	ZDMA_ATTR_TYPE     = 1 << 1,
	ZDMA_ATTR_INTR     = 1 << 2,
	ZDMA_ATTR_CMD      = 1 << 3,
};

enum {
	ZDMA_STATE_DISABLED = 0,
	ZDMA_STATE_ENABLED = 1,
	ZDMA_STATE_PAUSED = 2,
	ZDMA_STATE_ERROR = 3,
};

enum {
	ZDMA_MODE_RW = 0,
	ZDMA_MODE_WO = 1,
	ZDMA_MODE_RO = 2,
};

enum {
	ZDMA_PT_REG = 0,
	ZDMA_PT_MEM = 1,
};

enum {
	AXI_BURST_FIXED = 0,
	AXI_BURST_INCR = 1,
};

struct zdma_descr {
	uint64_t addr;
	uint32_t size;
        uint32_t attr;
};

/* Compute a base address for a given block and channel.  */
unsigned char *zdma_base(char *base, unsigned int ch);
void zdma_reset(char *base, unsigned int ch);
/* Wait for a given state.  */
int zdma_wait_for(char *base, unsigned int ch, int wstate);
/* Allocate a set of continous descriptors with proper alignment.  */
struct zdma_descr *zdma_descr_alloc(struct alloc_ctx *area,
				unsigned int nr, char **mem);
void zdma_descr_init(struct zdma_descr *d, void *addr,
			    unsigned int size,
			    unsigned int cmd,
			    bool intr, bool ptype, bool coherent);
/* Write a 64bit value into two contigous regs.  */
void zdma_write64(void *base, uint64_t val64);
