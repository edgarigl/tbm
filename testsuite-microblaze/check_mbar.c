/* Test cases for load reverse.  */
#include "sys.h"

#define mb_mbar(x) __asm__ __volatile__ ("\tmbar\t%0\n" : : "i" (x))

void check_mbar(void)
{
	printf("%s\n", __func__);
	/* ??? side effects not visible to C.
           Test compilation and runability  */
	mb_mbar(0);
	mb_mbar(1);
	mb_mbar(2);
	mb_mbar(3);
	mb_mbar(4);
	mb_mbar(5);
	mb_mbar(6);
	mb_mbar(7);
}
