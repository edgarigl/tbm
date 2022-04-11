#include <stdio.h>
#include "testcalls.h"

extern void check_apu_dual(void);
extern void check_crl(void);
extern void check_zdma(void);
extern void check_xrams(void);

void check_zynq3(void)
{
	printf("%s\n", __func__);
	check_apu_dual();
	check_crl();
	check_zdma();
	check_xrams();
}

__testcall(check_zynq3);
