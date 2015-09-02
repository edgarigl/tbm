#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

void a32_check_smc(void);
void a32_check_vttbr(void);

static void a32_test(void)
{
	printf("%s\n", __func__);
	a32_check_smc();
	a32_check_vttbr();
}

__testcall(a32_test);
