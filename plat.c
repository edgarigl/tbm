#include <stdint.h>
#include "sys.h"

__attribute__ ((weak)) void plat_cache_flush(void)
{
}

__attribute__ ((weak)) void plat_init(void)
{
}

__attribute__ ((weak)) void udelay(unsigned int us)
{
	volatile uint64_t d = us;

	d *= 128;
	while (d--)
		mb();
}
