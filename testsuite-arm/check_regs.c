#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void check_vpidr(bool has_el2)
{
	uint32_t scr;
	uint32_t v0, v1;

	printf("%s\n", __func__);
	scr = a32_get_scr();
	a32_set_scr(scr | SCR_NS);

	v0 = a32_get_vpidr();
	a32_set_vpidr(0x10000);
	v1 = a32_get_vpidr();

	printf("VPIDR v0=%x v1=%x\n", v0, v1);
	if (has_el2) {
		assert(v1 == 0x10000);
		a32_set_vpidr(v0);
		v1 = a32_get_vpidr();
		assert(v1 == v0);
	} else {
		assert(v1 == v0);
	}

	a32_set_scr(scr);
}

static void check_vmpidr(bool has_el2)
{
	uint32_t scr;
	uint32_t v0, v1;

	printf("%s\n", __func__);
	scr = a32_get_scr();
	a32_set_scr(scr | SCR_NS);

	v0 = a32_get_vmpidr();
	a32_set_vmpidr(0x10000);
	v1 = a32_get_vmpidr();

	printf("VMPIDR v0=%x v1=%x\n", v0, v1);
	if (has_el2) {
		assert(v1 == 0x10000);
		a32_set_vmpidr(v0);
		v1 = a32_get_vmpidr();
		assert(v1 == v0);
	} else {
		assert(v1 == v0);
	}

	a32_set_scr(scr);
}

static void check_vttbr(bool has_el2)
{
	uint32_t scr;
	uint64_t v;

	printf("%s\n", __func__);

	scr = a32_get_scr();
	a32_set_scr(scr | SCR_NS);

	a32_set_vttbr(0x10000);
	v = a32_get_vttbr();
	assert(v == 0x10000);

	a32_set_scr(scr);
}

void a32_check_regs(bool has_el2)
{
	printf("%s:\n", __func__);
	check_vttbr(has_el2);
	check_vpidr(has_el2);
	check_vmpidr(has_el2);
}
