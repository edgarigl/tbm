#include <stdio.h>
#include "testcalls.h"

extern void check_zdma(void);
extern void check_xrams(void);

void check_zynq3(void)
{
	printf("%s\n", __func__);
	check_zdma();
	check_xrams();
}

__testcall(check_zynq3);
