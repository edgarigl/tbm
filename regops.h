#ifndef __REGOPS_H___
#define __REGOPS_H___

#include "bitops.h"

static inline void reg32_waitformask(uint32_t *addr, uint32_t mask, uint32_t val)
{
	uint32_t r;

	do {
		r = readl(addr);
	} while ((r & mask) != val);
}

static inline void reg32_write64le(uint32_t *addr, uint64_t val)
{
	writel(addr, val);
	writel(addr + 1, val >> 32);
}

static inline uint64_t reg64_read(uint64_t *addr)
{
	uint64_t r;
	r = *(volatile uint64_t *) addr;
	return r;
}

static inline void reg64_write(uint64_t *addr, uint64_t val)
{
	*addr = val;
}

#define reg32_field(v, RG, RN, RF)				\
	bit_field32(v,						\
		RG ## _ ## RN ## _ ## RF ## _ ## SHIFT,		\
		RG ## _ ## RN ## _ ## RF ## _ ## WIDTH)
#endif
