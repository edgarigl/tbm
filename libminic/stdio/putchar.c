#include <unistd.h>

int putchar(int c)
{
	return write(1, &c, 1);
}

