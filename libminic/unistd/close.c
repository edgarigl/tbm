#include <unistd.h>

int close(int fd) {
#ifdef __cris__
	register int r asm("r10");
	asm ("moveq 6, $r9\n" /* NR_close.  */
	     "break 13\n" : :);
	return r;
#else
	return 0;
#endif
}

