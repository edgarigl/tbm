#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *path, struct stat *buf)
{
#ifdef __cris__
	register int r asm("r10");
	asm ("move.d 106, $r9\n" /* NR_fstat.  */
	     "break 13\n" : :);
	return r;
#else
	return -1;
#endif
}

int __xstat(int ver, const char *path, struct stat *buf)
{
	return stat(path, buf);
}

