#include <stdlib.h>
#include <assert.h>

char *strchr(const char *s, int c)
{
	assert(s);
	while (*s) {
		if (*s == c)
			break;
		s++;
	}
	return *s == c ? (char *) s : NULL;
}
