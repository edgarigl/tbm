#include <stdio.h>
#include <stdlib.h>
#include "sys.h"

void panic(const char *str)
{
	printf("Panic %s\n", str);
	while (1)
		cpu_wfi();
}

void hexdump(const char *prefix, const void *buf, size_t len)
{
	const unsigned char *u8 = buf;
	size_t i;

	if (prefix)
		printf("%s @ %p len=%u:\n", prefix, buf, (unsigned int) len);
	for (i = 0; i < len; i++) {
		if ((i % 16) == 0)
			printf("%u: ", (unsigned int) i);
		printf("%x ", u8[i]);
		if (((i + 1) % 16) == 0)
			putchar('\n');
	}
	putchar('\n');
}
