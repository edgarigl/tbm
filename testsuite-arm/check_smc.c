#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static inline uint32_t a32_get_scr(void)
{
	uint32_t scr;
	__asm__ __volatile__ ("mrc\tp15, 0, %0, c1, c1, 0\n" : "=r" (scr));
	return scr;
}

static inline void a32_set_scr(uint32_t scr)
{
	__asm__ __volatile__ ("mcr\tp15, 0, %0, c1, c1, 0\n" : : "r" (scr));
}

static inline uint64_t a32_get_hcr(void)
{
	union {
		uint32_t u32[2];
		uint64_t u64;
	} hcr;
	__asm__ __volatile__ ("mrc\tp15, 4, %0, c1, c1, 0\n" : "=r" (hcr.u32[0]));
	__asm__ __volatile__ ("mrc\tp15, 4, %0, c1, c1, 4\n" : "=r" (hcr.u32[1]));
	return hcr.u64;
}

static inline void a32_set_hcr(uint64_t v)
{
	union {
		uint32_t u32[2];
		uint64_t u64;
	} hcr = { .u64 = v };
	__asm__ __volatile__ ("mcr\tp15, 4, %0, c1, c1, 0\n" : : "r" (hcr.u32[0]));
	__asm__ __volatile__ ("mcr\tp15, 4, %0, c1, c1, 4\n" : : "r" (hcr.u32[1]));
}

static void check_hcr_mask(void)
{
#if 0
	uint64_t hcr, hcr_prev;
	hcr_prev = a32_get_hcr();

	a32_set_hcr(~0);
	hcr = a32_get_hcr();
	printf("hcr=%lx\n", hcr);
	assert(hcr == 0x33ff);

	a32_set_hcr(0);
	hcr = a32_get_hcr();
	printf("hcr=%lx\n", hcr);
	assert(hcr == 0x0);

	a32_set_hcr(hcr_prev);
#endif
}

static void check_scr_mask(void)
{
	uint32_t scr, scr_prev;
	scr_prev = a32_get_scr();

	a32_set_scr(~0);
	scr = a32_get_scr();
	printf("scr=%lx\n", scr);
	assert(scr == 0x33ff || scr == 0x3fff);

	a32_set_scr(0);
	scr = a32_get_scr();
	printf("scr=%lx\n", scr);
	assert(scr == 0x0);

	a32_set_scr(scr_prev);
}

void a32_check_smc(void)
{
	printf("%s:\n", __func__);

	check_scr_mask();
	check_hcr_mask();
}
