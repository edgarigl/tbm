#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t strlen(const char *s) {
	int i = 0;
	while (s[i])
		i++;
	return i;
}
