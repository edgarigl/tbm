#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int fclose(FILE *fp)
{
	return close((intptr_t) fp);
}

