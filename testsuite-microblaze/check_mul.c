#include <stdio.h>
#include "sys.h"

int uart_putchar(int c);

struct multest_t {
	unsigned int a, b, r;
};

volatile struct multest_t mulops[] =
{
	{10, 2, 10 * 2},
	{10, 3, 10 * 3},
	{10, 4, 10 * 4},
#if 1
	{10, 5, 10 * 5},
	{10, 6, 10 * 6},
	{10, 7, 10 * 7},
	{123, 333, 123 * 333},
	{123, 33333, 123 * 33333},
	{-10, 0xffff, -10 * 0xffff},
#endif
};


void check_mul(void)
{
#if 1
	unsigned int i;

	puts(__func__);
	for (i = 0; i < sizeof (mulops) / sizeof mulops[0]; i++)
	{
		puthex(i); uart_putchar(' ');
		puthex(mulops[i].a); uart_putchar('*');
		puthex(mulops[i].b); uart_putchar('=');
		if (mulops[i].a * mulops[i].b != mulops[i].r)
			err();
#if 1
		puthex(mulops[i].r); uart_putchar('\n');
#endif
	}
#endif
}
