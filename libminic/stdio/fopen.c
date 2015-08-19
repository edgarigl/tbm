#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *fopen(const char *path, const char *mode)
{
	int fd;
	fd = open(path, O_RDWR, 0);
	return (void *) (intptr_t) fd;
}

