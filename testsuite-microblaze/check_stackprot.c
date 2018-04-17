/* Test cases for load reverse.  */
#include "sys.h"

void set_slr(uintptr_t p)
{
	__asm__ __volatile__ ("mts\trslr, %0\n" : : "r" (p));
}

uintptr_t get_slr(void)
{
	uintptr_t r;
	__asm__ __volatile__ ("mfs\t%0, rslr\n" : "=r" (r));
	return r;
}

void set_shr(uintptr_t p)
{
	__asm__ __volatile__ ("mts\trshr, %0\n" : : "r" (p));
}

uintptr_t get_shr(void)
{
	uintptr_t r;
	__asm__ __volatile__ ("mfs\t%0, rshr\n" : "=r" (r));
	return r;
}

void check_stackprot(void)
{
	uint32_t slr, shr;

	slr = get_slr();
	shr = get_shr();

	printf("%s slr=%x shr=%x\n", __func__, slr, shr);
	/* Enable and check!.  */
	set_slr(4);
	if (get_slr() != 4)
		err();
	set_shr(0xfffffff0);
	if (get_shr() != 0xfffffff0)
		err();

	/* Restore.  */
	set_slr(slr);
	set_shr(shr);
}
