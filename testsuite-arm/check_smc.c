#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

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
