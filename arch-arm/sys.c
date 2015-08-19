#include <stdio.h>
#include <stdint.h>
#include "sys.h"

unsigned long __stack_chk_guard = 0;

void __stack_chk_fail(void)
{
	puts(__func__);
	err();
}

/* Dummy.  */
__attribute__ ((weak)) void cpu_test(void)
{
}

__attribute__ ((weak)) void app_run(void)
{
}
