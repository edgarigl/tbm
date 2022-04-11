/*
 * Smoke tests for the Everest CRL.
 * Written by Edgar E. Iglesias.
 */

#define _MINIC_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "regops.h"

#include "plat-memmap.dtsh"
#include "drivers/versal/reg-apu_dual.h"

void check_apu_dual(void)
{
	uint32_t v;

	printf("%s\n", __func__);

	v = readl(MM_FPD_FPD_APU + R_APU_DUAL_PWRCTL);
	v |= APU_DUAL_PWRCTL_L2FLUSHREQ_MASK;
	writel(MM_FPD_FPD_APU + R_APU_DUAL_PWRCTL, v);

	v &= ~APU_DUAL_PWRCTL_L2FLUSHREQ_MASK;
	v |= APU_DUAL_PWRCTL_CLREXMONREQ_MASK;
	writel(MM_FPD_FPD_APU + R_APU_DUAL_PWRCTL, v);
}
