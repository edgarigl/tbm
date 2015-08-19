#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int fputc(int c, FILE *stream)
{
	return write((intptr_t) stream, &c, 1);
}
