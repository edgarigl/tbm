/*
 * Versal RPU release from reset.
 * Written by Edgar E. Iglesias.
 */
#define _MINIC_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include "sys.h"

#include "plat-memmap.dtsh"
#include "drivers/versal/reg-crl.h"

void app_run(void)
{
	/* Release both R5's from reset.  */
	printf("Release both R5s from reset\n");
	writel(MM_CRL + R_CRL_RST_CPU_R5, 0);
	printf("APU halt\n");
        while (1) {
		cpu_wfi();
        }
}
