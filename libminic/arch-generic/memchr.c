#include <stdlib.h>
#include <string.h>

void *memchr(const void *s, int c, size_t n)
{
	unsigned char *p;
	unsigned char v = c;

	while (n) {
		if (*p == v)
			break;
		n--;
		p++;
	}
	return n ? p : NULL;
}
