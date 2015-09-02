#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

void check_mmu(void);
void a64_check_timer(void);
void a64_check_smc(void);
void a64_check_el_switch(void);

static void a64_check_idef_ldr(uint64_t val)
{
	uint64_t x1;

        if ((val + 4) == (uintptr_t) &val) {
		return;
	}

	printf("%s ldr w1, [x1], #4\n", __func__);
	__asm__ __volatile__("mov x1, %1\n"
			     "ldr w1, [x1], #4\n"
			     "mov %0, x1\n" : "=r" (x1) : "r" (&val));
	printf("x1 = %llx\n", x1);
	if (x1 == (val & 0xffffffff)) {
		printf("WBsupressed\n");
	} else if ((x1 - 4) == (uintptr_t) &val) {
		printf("write-back unknown: resulting in addr\n");
	} else {
		printf("unkown\n");
	}
}

static void a64_test(void)
{
	unsigned int el;
	el = aarch64_current_el();

	printf("%s EL=%d\n", __func__, el);
	a64_check_idef_ldr(0xeddeeddeeddeULL);
	a64_check_smc();
	a64_check_el_switch();
//	a64_check_timer();
	check_mmu();
}

__testcall(a64_test);
