#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream)
{
	return write((intptr_t) stream, ptr, size * nmemb);
}

