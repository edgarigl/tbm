#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void a64_check_raise_el(void)
{
	unsigned int el;

	if (aarch64_get_highest_el() != 3) {
		/* We can't raise the EL.  */
		return;
	}

	printf("%s\n", __func__);
	el = aarch64_current_el();
	printf("EL=%d\n", el);
	assert(el == EL1);

	aarch64_raise_el(EL2);
	el = aarch64_current_el();
	printf("EL=%d\n", el);
	assert(el == EL2);

	aarch64_raise_el(EL3);
	el = aarch64_current_el();
	printf("EL=%d\n", el);
	assert(el == EL3);
}

static void a64_check_drop_el(void)
{
	unsigned int el;
	uint32_t scr;
	uint64_t hcr;

	printf("%s\n", __func__);
	el = aarch64_current_el();
	printf("3-EL=%d\n", el);

	if (el == 3) {
		aarch64_mrs(scr, "scr_el3");
		scr |= SCR_EA | SCR_RW | SCR_NS;
		aarch64_msr("scr_el3", scr);

		asm volatile ("msr\tsctlr_el2, %0\n" : : "r" (0));
		aarch64_drop_el(EL2, SPXH);
		el = aarch64_current_el();
		printf("2-EL=%d\n", el);
		assert(el == EL2);
	}

	if (el == 2) {
		aarch64_mrs(hcr, "hcr_el2");
		hcr |= HCR_RW | HCR_AMO;
		aarch64_msr("hcr_el2", hcr);

		asm volatile ("msr\tsctlr_el1, %0\n" : : "r" (0));
		aarch64_drop_el(EL1, SPXH);
		el = aarch64_current_el();
		printf("1-EL=%d\n", el);
		assert(el == EL1);
	}

	if (el == 1 && 0) {
		aarch64_drop_el(EL0, SP0);
//		el = aarch64_current_el();
//		printf("EL=%d\n", el);
//		assert(el == EL0);
//		aarch64_raise_el(EL1);
	}
}

void a64_check_el_switch(void)
{
	unsigned int el;
	el = aarch64_current_el();

	printf("%s EL=%d\n", __func__, el);
	a64_check_drop_el();
	a64_check_raise_el();
}
