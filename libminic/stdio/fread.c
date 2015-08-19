#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return read((intptr_t) stream, ptr, size * nmemb);
}
