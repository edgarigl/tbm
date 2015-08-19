#include <stdarg.h>
#include <stdint.h>
#include "sys.h"

static unsigned int a[16];
static char *g_fmt;

static void va_test(char *format, ...)
{
        va_list va;

        va_start(va, format);
	g_fmt = format;
	a[0] = va_arg(va, unsigned int);
	a[1] = va_arg(va, unsigned int);
	a[2] = va_arg(va, unsigned int);
	a[3] = va_arg(va, unsigned int);
	a[4] = va_arg(va, unsigned int);
	a[5] = va_arg(va, unsigned int);
	a[6] = va_arg(va, unsigned int);
	a[7] = va_arg(va, unsigned int);
	a[8] = va_arg(va, unsigned int);
        va_end(va);
        return;
}

extern int uart_putchar(int c);

void check_varargs2(void)
{
	puts(__func__);
	va_test("kalle", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
#if 1
	puthex((uintptr_t) "kalle"); uart_putchar('\n');
	puthex((uintptr_t) g_fmt); uart_putchar('\n');
	puthex(a[0]); uart_putchar('\n');
	puthex(a[1]); uart_putchar('\n');
	puthex(a[2]); uart_putchar('\n');
	puthex(a[3]); uart_putchar('\n');
	puthex(a[4]); uart_putchar('\n');
#endif
	if (g_fmt != (char *) "kalle")
		err();
	if (a[0] != 1)
		err();
	if (a[1] != 2)
		err();
	if (a[2] != 3)
		err();
	if (a[3] != 4)
		err();
	if (a[4] != 5)
		err();
	if (a[5] != 6)
		err();
	if (a[6] != 7)
		err();
	if (a[7] != 8)
		err();
	if (a[8] != 9)
		err();
}
