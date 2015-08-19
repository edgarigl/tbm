#include "sys.h"

extern unsigned long
ed_udivmodsi4(unsigned long num, unsigned long den);

struct divtest_t {
	unsigned int a, b, r;
};

volatile struct divtest_t ops[] =
{
	{10, 2, 10 / 2},
	{10, 3, 10 / 3},
	{10, 4, 10 / 4},
	{10, 5, 10 / 5},
	{10, 6, 10 / 6},
	{10, 7, 10 / 7},
	{123, 333, 123 / 333},
	{123, 33333, 123 / 33333},
};

void check_div(void)
{
	unsigned int i;

	puts(__func__);
	for (i = 0; i < sizeof (ops) / sizeof ops[0]; i++)
	{
		if (ops[i].a / ops[i].b != ops[i].r)
			err();
	}
}
