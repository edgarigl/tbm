#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int __fxstat(int ver, int fd, struct stat *buf)
{
#ifdef __cris__
	register int r asm("r10");
	asm ("move.d $r11, $r10\n"
	     "move.d $r12, $r11\n"
	     "move.d 108, $r9\n" /* NR_fstat.  */
	     "break 13\n" : :);
	return r;
#else
	return -1;
#endif
}

int fstat(int fd, struct stat *buf)
{
	return __fxstat(0, fd, buf);
}


