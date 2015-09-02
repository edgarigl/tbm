#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void check_vttbr(void)
{
	uint32_t scr;
	uint64_t v;

	scr = a32_get_scr();
	a32_set_scr(scr | SCR_NS);

	a32_set_vttbr(0x10000);
	v = a32_get_vttbr();
	assert(v = 0x10000);

	a32_set_scr(scr);
}

void a32_check_vttbr(void)
{
	printf("%s:\n", __func__);

	check_vttbr();
}
