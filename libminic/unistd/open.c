#include <sys/types.h>

int open(const char *pathname, int flags, mode_t mode) {
#ifdef __cris__
	register int r asm("r10");
	asm ("moveq 5, $r9\n" /* NR_open.  */
	     "break 13\n" : :);
	return r;
#else
	return -1;
#endif
}
