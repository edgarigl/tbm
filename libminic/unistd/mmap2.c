#include <stddef.h>
#include <unistd.h>

void * mmap2(void *addr, size_t length, int prot, int flags,
	    int fd, unsigned long offset)
{
#ifdef __cris__
	register void *r asm("r10");
	asm ("move.d [$sp], $r9\n"
	     "move $r9, $mof\n"
	     "subq  4, $sp\n"
	     "move $srp, $r9\n"
	     "move.d $r9, [$sp]\n"
	     "move.d $sp, $r9\n"
	     "addq 8, $r9\n"
	     "move.d [$r9], $r9\n"
	     "move $r9, $srp\n"
	     "move.d 192, $r9\n" /* NR_mmap2.  */
	     "break 13\n"
	     "move.d [$sp+], $r9\n"
	     "move $r9, $srp\n"
		 : : );
	return r;
#else
	return (void *) -1;
#endif
}
