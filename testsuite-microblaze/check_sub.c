#include "sys.h"

int SI_sub(int a, int b)
{
	return a - b;
}

unsigned int USI_sub(unsigned int a, unsigned int b)
{
	return a - b;
}


void check_c_sub(void)
{
	if (SI_sub (10, 10) != 0)
		err();
	if (SI_sub (10, 20) != -10)
		err();
	if (SI_sub (20, 10) != 10)
		err();
	if (USI_sub (10, 10) != 0)
		err();
	if (USI_sub (10, 20) != (unsigned int) -10)
		err();
	if (USI_sub (20, 10) != 10)
		err();
}
