#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

void a32_check_smc(void);

static void a32_test(void)
{
	printf("%s\n", __func__);
	a32_check_smc();
}

__testcall(a32_test);
