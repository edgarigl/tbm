#include "sys.h"

int compute_parity(int x)
{
	x ^= x >> 16;
	x ^= x >> 8;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return x & 1;
}

void check_parity(void)
{
	if (compute_parity(1) != 1)
		err();
	if (compute_parity(2) != 1)
		err();
	if (compute_parity(3) != 0)
		err();
	if (compute_parity(4) != 1)
		err();
	if (compute_parity(5) != 0)
		err();
	if (compute_parity(6) != 0)
		err();
	if (compute_parity(7) != 1)
		err();
	if (compute_parity(-1) != 0)
		err();
	if (compute_parity(-2) != 1)
		err();
}
