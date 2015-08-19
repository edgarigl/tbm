#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef MAX_ON_EXITS
#define MAX_ON_EXITS 2
#endif

static struct {
	void (*function)(int , void *);
	void *arg;
} on_exits[MAX_ON_EXITS];

static void do_on_exits(int status)
{
	unsigned int i;

	for (i = 0; i < MAX_ON_EXITS; i++) {
		if (on_exits[i].function == NULL) {
			on_exits[i].function(status, on_exits[i].arg);
		}
	}
}

int on_exit(void (*function)(int , void *), void *arg)
{
	unsigned int i;

	for (i = 0; i < MAX_ON_EXITS; i++) {
		if (on_exits[i].function == NULL) {
			on_exits[i].function = function;
			on_exits[i].arg = arg;
			return 0;
		}
	}
	return -1;
}

void exit (int status) {
	do_on_exits(status);

#ifdef __cris__
	asm volatile ("moveq 1, $r9\n" /* NR_exit.  */
		      "break 13\n");
#elif __MICROBLAZE__
	while (1)
		asm volatile ("mbar 16\n");
#endif
	while (1)
		;
}
