#include <unistd.h>
#include <string.h>

int  __attribute__((weak)) write(int fd, const void *buf, size_t count) {
	return count;
}

