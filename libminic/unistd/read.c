#include <unistd.h>

int read(int fd, void *buf, size_t count) {
#ifdef __cris__
	register int r asm("r10");
	asm ("moveq 3, $r9\n" /* NR_read.  */
	     "break 13\n" : :);
	return r;
#else
	return -1;
#endif
}

