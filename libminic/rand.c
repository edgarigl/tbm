/* just a hack for testsuites, do not use for security purposes.  */
#include <stdlib.h>
#include <limits.h>

static unsigned int rseed;

int rand_r(unsigned int *seedp)
{
	unsigned int rnd;

	/* Lower 16bits.  */
	*seedp = *seedp * 1103515245 + 12345;
	rnd = *seedp;
	rnd >>= 16;

	/* Upper 16bits.  */
	*seedp = *seedp * 1103515245 + 12345;
	rnd |= (*seedp >> 16) << 16;

	rnd &= RAND_MAX;
	return rnd;
}

int rand(void)
{
	return rand_r(&rseed);
}

void srand(unsigned int seed)
{
	rseed = seed;
}
