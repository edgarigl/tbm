#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
	printf("hcr=%llx\n", hcr);
	assert(hcr == 0x33ff);

	a32_set_hcr(0);
	hcr = a32_get_hcr();
	printf("hcr=%llx\n", hcr);
	assert(hcr == 0x0);

	a32_set_hcr(hcr_prev);
#endif
}

static bool check_scr_mask(void)
{
	bool has_el2;
	uint32_t scr, scr_prev;
	scr_prev = a32_get_scr();

	a32_set_scr(~0);
	scr = a32_get_scr();
	printf("scr=%x\n", scr);
	/*
 	 * AArch32 ARMv8: Has no SCR.RW and SCR.ST = 0x33ff
	 * AArch64 ARMv8: Has everything 0x3fff
	 * AArch64 ARMv8 no EL2: Has no HCE 0x3eff
	 * ARMv7: Has all = 0x3fff
	 * ARMv7 no EL2: has no HCE nor SMD 0x3e7f
	 */
	switch (scr) {
	case 0x33ff:
	case 0x3fff:
		has_el2 = true;
		break;
	case 0x3eff:
	case 0x3e7f:
		has_el2 = false;
		break;
	default:
		has_el2 = false;
		assert(0);
		break;
	}

	a32_set_scr(0);
	scr = a32_get_scr();
	printf("scr=%x\n", scr);
	assert(scr == 0x0);

	a32_set_scr(scr_prev);
	return has_el2;
}

bool a32_check_smc(void)
{
	bool has_el2 = false;
	printf("%s:\n", __func__);

	if (__ARM_ARCH == 8) {
		has_el2 = check_scr_mask();
		if (has_el2) {
			check_hcr_mask();
		}
	}
	return has_el2;
}
