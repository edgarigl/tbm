#include <string.h>

void *memset(void *s, int c, size_t n)
{
	char *ss = s, *end;

	end = ss + n;
	while (ss < end)
		*ss++ = c;
	return s;
}
