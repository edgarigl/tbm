#include <stdio.h>
#include "testcalls.h"
#include "ronaldo-version.h"

extern void check_zdma(void);

void check_ronaldo(void)
{
	printf("%s\n", __func__);
	check_zdma();
}

__testcall(check_ronaldo);
