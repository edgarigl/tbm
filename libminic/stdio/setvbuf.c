#include <stdio.h>
#include <stdlib.h>

int setvbuf(FILE *stream, char *buf, int mode, size_t size) __attribute__ ((weak));
int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
	return 1;
}

