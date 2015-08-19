#define toupper dut_toupper
#include "../ctype.c"
#undef toupper

#include <assert.h>
#define FATALD(x)
#define PROFILE_MALLOC 1

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define fail_unless(x)                                         \
do                                                             \
{                                                              \
  if (!(x)) {                                                  \
    fprintf (stderr, "FAILED at %s:%d\n", __FILE__, __LINE__); \
    exit (EXIT_FAILURE);                                       \
  }                                                            \
} while (0);


/* Just brute force check the entire space :)  */
static void check_toupper(void)
{
	int i;

	printf("%s\n", __func__);

	/* man toupper:
	   If  c  is  not  an  unsigned char value, or EOF, the behaviour
	   of these functions is undefined. */

	/* EOF.  */
	i = EOF;
	if (dut_toupper(i) != toupper(i)) {
		fprintf(stderr, "failed %d %d %d  eof=%d\n",
			i, dut_toupper(i), toupper(i), EOF);
		assert(0);
	}

	/* scan the rest of the allowed space.  */
	for (i = 0; i < UCHAR_MAX; i++) {
		if (dut_toupper(i) != toupper(i)) {
			fprintf(stderr, "failed %d %d %d  eof=%d\n",
				i, dut_toupper(i), toupper(i), EOF);
			assert(0);
		}

		if ((i & ((1 << 24) - 1)) == 0)
			fprintf(stderr, ".");
	}
	printf("\n%s OK\n", __func__);
}

int main(void)
{
	srand(0xeddebeef);
	check_toupper();
	return 0;
}
