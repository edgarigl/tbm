#define strlen dut_strlen
#include "../arch-generic/strlen.c"
#undef strlen

#include <assert.h>
#define FATALD(x)
#define PROFILE_MALLOC 1

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t strlen(const char *s);


#define fail_unless(x)                                         \
do                                                             \
{                                                              \
  if (!(x)) {                                                  \
    fprintf (stderr, "FAILED at %s:%d\n", __FILE__, __LINE__); \
    exit (EXIT_FAILURE);                                       \
  }                                                            \
} while (0);

#ifndef MAX_ALLOC
#define MAX_ALLOC	(64 * 1024)
#endif

#ifndef MAX_ROUNDS
#define MAX_ROUNDS	(64 * 1024)
#endif

static void check_strlen(void)
{
	int i;

	printf("%s\n", __func__);

	for (i = 0; i < MAX_ROUNDS; i++) {
		unsigned int size;
		unsigned int p;
		char *s;

		size = rand();
		/* from 0 to 32K.  */
		size &= (MAX_ALLOC) - 1;
		s = malloc(size + 1);
		assert(s);

		/* Populate the string.  */
		for (p = 0; p <= size; p++) {
			char ch = 0;

			while (ch == 0)
				ch = rand() & 0xff;
			s[p] = ch;
		}
		s[size] = 0;

		if (dut_strlen(s) != size) {
			printf("dut_strlen=%zd strlen=%zd size=%u\n",
				dut_strlen(s), strlen(s), size);
		}
		free(s);
		if ((i & 511) == 0)
			fprintf(stderr, ".");
	}
	printf("\n%s OK\n", __func__);
}

int main(void)
{
	srand(0xeddebeef);
	check_strlen();
	return 0;
}
