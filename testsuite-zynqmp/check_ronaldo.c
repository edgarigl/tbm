#include <stdio.h>
#include "testcalls.h"
#include "ronaldo-version.h"

extern void check_bram_exec(void);
extern void check_zdma(void);
extern void check_qspi(void);

void check_ronaldo(void)
{
	printf("%s\n", __func__);
	check_bram_exec();
	check_zdma();
	check_qspi();
}

__testcall(check_ronaldo);
