#include <string.h>
#include <unistd.h>

int puts(const char *s)
{
	static const char nl = '\n';
	write(1, s, strlen(s));
	write(1, &nl, 1);
	return 1;
}

