#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

#define UART_BASEADDR ((char *) 0x84000000)
#define R_ADDR (4 * 4)
#define R_VAL (5 * 4)
#define R_WRITE (6 * 4)
#define R_RAMCFG (7 * 4)
#define R_IOTEST (8 * 4)
static void check_iotest(void)
{
	printf("iotest\n");
	while (1) {
		writel(UART_BASEADDR + R_IOTEST, 0);
	}
}

__testcall(check_iotest);
