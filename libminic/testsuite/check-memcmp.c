#define memcmp dut_memcmp
#include "../arch-generic/memcmp.c"
#undef memcmp

#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int memcmp(const void *s1, const void *s2, size_t n);

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

static void check_memcmp(void)
{
	int i;

	printf("%s\n", __func__);

	for (i = 0; i < MAX_ROUNDS; i++) {
		unsigned int size;
		unsigned int *b1, *b2;
		unsigned int p;
		unsigned int diff;
		int a, b;

		size = rand() % (MAX_ALLOC / 4);
		size += 1;
		b1 = malloc(size * 4);
		b2 = malloc(size * 4);
		assert(b1);
		assert(b2);

		for (p = 0; p < size; p++) {
			b1[p] = b2[p] = rand();
		}

		diff = rand() & 1;
		if (diff) {
			b1[rand() % size] = rand();
		}

		a = memcmp(b1, b2, size);
		b = dut_memcmp(b1, b2, size);

		if (a != b && ((a < 0) != (b < 0))) {
			fprintf(stderr, "size=%d diff=%d %d %d\n", size, diff,
				memcmp(b1, b2, size), dut_memcmp(b1, b2, size));
			assert(0);
		}

		free(b1);
		free(b2);
		if ((i & ((1 << 9) - 1)) == 0)
			fprintf(stderr, ".");
	}
	printf("\n%s OK\n", __func__);
}

int main(void)
{
	srand(0xeddebeef);
	check_memcmp();
	return 0;
}
