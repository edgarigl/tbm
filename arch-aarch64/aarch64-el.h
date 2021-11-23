#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "sys.h"

enum aarch64_el {
	EL0 = 0,
	EL1 = 1,
	EL2 = 2,
	EL3 = 3,
};

enum {
	SP0  = 0,
	SPXH = 1,
};

static inline enum aarch64_el aarch64_current_el(void)
{
	unsigned int el;

	asm volatile ("mrs\t%0, CurrentEL\n" : "=r" (el));
	return (el >> 2) & 0x3;
}

void aarch64_drop_el(const enum aarch64_el el, bool stack_h);
void aarch64_raise_el(unsigned int target_el);

extern unsigned int aarch64_hel;
static inline unsigned int aarch64_get_highest_el(void)
{
	return aarch64_hel;
}
