#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include "testcalls.h"
#include "sys.h"
#include "libverif/verif.h"
#include "libverif/verif.h"

extern void plat_init(void);
extern void uart_init(void);
extern int uart_putchar(int c);

#define xstr(s) str(s)
#define str(s) #s

static const char arch_str[] = xstr(__BOARD__) "-" xstr(__UNIT__);

/* Override the default write from libc.  */
int write(int fd, const void *buf, size_t count)
{
	char *cbuf = (char *) buf;
	size_t pos = 0;

	while (count > 0) {
		uart_putchar(cbuf[pos++]);
		count--;
	}
	return pos;
}
int main(void)
{
	int nr_tests;

	uart_init();

	printf("\nTBM %s %s running on %s\n", __DATE__, __TIME__, arch_str);

	arch_init();
	plat_init();

	nr_tests = __testcalls_exec();

	if (nr_tests)
		printf("\n** %s: All %d directed testsuites passed.\n", arch_str, nr_tests);

	app_run();

	printf("** %s: Run CRT tests (may run forver)\n", arch_str);
	crt_run_all();

	puts("\n\t- halt\n");
	while (1)
		cpu_wfi();

	return 0;
}
