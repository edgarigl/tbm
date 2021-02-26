#include <stdio.h>
#include "testcalls.h"

extern void check_zdma(void);

void check_zynq3(void)
{
	printf("%s\n", __func__);
	check_zdma();
}

__testcall(check_zynq3);
