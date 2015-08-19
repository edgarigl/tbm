#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int printf(const char *format, ...) __attribute__ ((weak));
int printf(const char *format, ...)
{
	va_list ap;
	int r;

	va_start(ap, format);
	r = vprintf(format, ap);
	va_end(ap);
	return r;
}

