#define TEST_GNU_BCOPY

#define bcopy dut_bcopy
#define bzero dut_bzero
#define memmove dut_memmove
#include "../memmove.c"
#undef memmove
#undef bzero

#include <assert.h>
#define FATALD(x)
#define PROFILE_MALLOC 1

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *memmove(void *dest, const void *src, size_t n);

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
#define MAX_ROUNDS	(128 * 1024)
#endif

static void check_memmove(void)
{
	int i;

	printf("%s\n", __func__);

	for (i = 0; i < MAX_ROUNDS; i++) {
		unsigned int size;
		unsigned int p, ps, pd = 0, len;
		unsigned int fw;
		char *s1, *s2;

		size = rand();
		/* from 0 to 32K.  */
		size &= (MAX_ALLOC) - 1;
		size += 4;
		s1 = malloc(size);
		s2 = malloc(size);
		assert(s1 && s2);

		/* Populate the string.  */
		for (p = 0; p < size; p++) {
			char ch = 0;
			ch = rand() & 0xff;
			s1[p] = ch;
			s2[p] = ch;
		}

		ps = rand() % (size - 1);
		fw = rand() & 1;

		/* Move forward or backwards ?  */
		if (fw || !ps) {
			pd = ps + (rand() % (size - ps));
			len = rand() % (size - pd);
		} else {
			pd = rand() % ps;
			len = rand() % (size - ps);
		}

		assert(memmove(s1 + pd, s1 + ps, len) == s1 + pd);
		assert(dut_memmove(s2 + pd, s2 + ps, len) == s2 + pd);
		assert(memcmp(s1, s2, size) == 0);

		free(s1);
		free(s2);
		if ((i & 127) == 0)
			fputc(fw ? '>' : '<', stderr);
	}
	printf("\n%s OK\n", __func__);
}

int main(void)
{
	srand(0xeddebeef);
	check_memmove();
	return 0;
}
