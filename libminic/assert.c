/*
 * libminic - assert.
 *
 * Written by Edgar E. Iglesias
 */
#include <stdio.h>

void
__assert (const char *str
#ifndef TINY_ASSERT
	  , unsigned int linenr
#endif
)
{
#ifdef TINY_ASSERT
	/* Show some error? if we printf here, the entire printf code will
	   be linked into apps using assert bloating the boot code.  */
	puts(str);
#else
	printf(str, linenr);
#endif

	/* Hlt.  */
	while (1)
		;
}
