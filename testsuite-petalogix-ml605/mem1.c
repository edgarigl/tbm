#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void check_aspace(void)
{
	uint32_t *p = (void *) 0x0;

	printf("initialize the bram\n");
	*p = 0xeddebeef;
}

__testcall(check_aspace);
