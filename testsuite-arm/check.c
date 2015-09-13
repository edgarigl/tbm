#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

bool a32_check_smc(void);
void a32_check_regs(bool has_el2);

static void a32_test(void)
{
	bool has_el2;

	printf("%s\n", __func__);
	has_el2 = a32_check_smc();
	a32_check_regs(has_el2);
}

__testcall(a32_test);
