/*
 * Test cases for MB bitfield insns.
 * Written by Edgar E. Iglesias <edgari@xilinx.com>
 */

#include <stdlib.h>
#include "sys.h"

#ifdef MB_HAS_BITFIELD_INSNS

static uint32_t model_bsifi(uint32_t d, uint32_t v, uint32_t w, uint32_t s)
{
	uint32_t mask = ((1UL << (w)) - 1) << s;
	uint32_t r = d;

	r = r & ~mask;
	r |= (v << (s)) & mask;
	return r;
}

static uint32_t model_bsefi(uint32_t v, uint32_t w, uint32_t s)
{
	uint32_t mask = (1UL << w) - 1;
	uint32_t r;

	r = (v >> s) & mask;
	return r;
}

#define CHECK_BSIFI(d, v, w, s)						\
if ((s + w) < 31) {							\
	uint32_t m_r = model_bsifi(d, v, w, s);				\
	uint32_t r = d;							\
									\
	__asm__ __volatile__ ("\tbsifi\t%0, %1, %2, %3\n"		\
		: "=r" (r) : "r" (v), "i" (w), "i" (s), "0" (r));	\
									\
	if (r != m_r) {							\
		if (0)							\
		printf("r=%x m_r=%x v=%x w=%d s=%d\n",			\
			r, m_r, v, w, s);				\
		err();							\
	}								\
}

#define CHECK_BSEFI(v, w, s)						\
if ((s + w) < 31) {							\
	uint32_t m_r = model_bsefi(v, w, s);				\
	uint32_t r;							\
									\
	__asm__ __volatile__ ("\tbsefi\t%0, %1, %2, %3\n"		\
			: "=r" (r) : "r" (v), "i" (w), "i" (s));	\
									\
	if (r != m_r) {							\
		if (0)							\
		printf("r=%x m_r=%x v=%x w=%d s=%d\n",			\
			r, m_r, v, w, s);				\
		err();							\
	}								\
}

#define GEN_BSIFI_S(d, v, w)						\
		CHECK_BSIFI(d, v, w, 0);				\
		CHECK_BSIFI(d, v, w, 1);				\
		CHECK_BSIFI(d, v, w, 2);				\
		CHECK_BSIFI(d, v, w, 3);				\
		CHECK_BSIFI(d, v, w, 4);				\
		CHECK_BSIFI(d, v, w, 5);				\
		CHECK_BSIFI(d, v, w, 6);				\
		CHECK_BSIFI(d, v, w, 7);				\
		CHECK_BSIFI(d, v, w, 8);				\
		CHECK_BSIFI(d, v, w, 9);				\
		CHECK_BSIFI(d, v, w, 10);				\
		CHECK_BSIFI(d, v, w, 11);				\
		CHECK_BSIFI(d, v, w, 12);				\
		CHECK_BSIFI(d, v, w, 13);				\
		CHECK_BSIFI(d, v, w, 14);				\
		CHECK_BSIFI(d, v, w, 15);				\
		CHECK_BSIFI(d, v, w, 16);				\
		CHECK_BSIFI(d, v, w, 17);				\
		CHECK_BSIFI(d, v, w, 18);				\
		CHECK_BSIFI(d, v, w, 19);				\
		CHECK_BSIFI(d, v, w, 20);				\
		CHECK_BSIFI(d, v, w, 21);				\
		CHECK_BSIFI(d, v, w, 22);				\
		CHECK_BSIFI(d, v, w, 23);				\
		CHECK_BSIFI(d, v, w, 24);				\
		CHECK_BSIFI(d, v, w, 25);				\
		CHECK_BSIFI(d, v, w, 26);				\
		CHECK_BSIFI(d, v, w, 27);				\
		CHECK_BSIFI(d, v, w, 28);				\
		CHECK_BSIFI(d, v, w, 29);				\
		CHECK_BSIFI(d, v, w, 31);				\

#define GEN_BSIFI_LARGE(d, v)						\
		GEN_BSIFI_S(d, v, 2);					\
		GEN_BSIFI_S(d, v, 3);					\
		GEN_BSIFI_S(d, v, 4);					\
		GEN_BSIFI_S(d, v, 5);					\
		GEN_BSIFI_S(d, v, 6);					\
		GEN_BSIFI_S(d, v, 7);					\
		GEN_BSIFI_S(d, v, 8);					\
		GEN_BSIFI_S(d, v, 9);					\
		GEN_BSIFI_S(d, v, 10);					\
		GEN_BSIFI_S(d, v, 11);					\
		GEN_BSIFI_S(d, v, 12);					\
		GEN_BSIFI_S(d, v, 13);					\
		GEN_BSIFI_S(d, v, 14);					\
		GEN_BSIFI_S(d, v, 15);					\
		GEN_BSIFI_S(d, v, 16);					\
		GEN_BSIFI_S(d, v, 17);					\
		GEN_BSIFI_S(d, v, 18);					\
		GEN_BSIFI_S(d, v, 19);					\
		GEN_BSIFI_S(d, v, 20);					\
		GEN_BSIFI_S(d, v, 21);					\
		GEN_BSIFI_S(d, v, 22);					\
		GEN_BSIFI_S(d, v, 23);					\
		GEN_BSIFI_S(d, v, 24);					\
		GEN_BSIFI_S(d, v, 25);					\
		GEN_BSIFI_S(d, v, 26);					\
		GEN_BSIFI_S(d, v, 27);					\
		GEN_BSIFI_S(d, v, 28);					\
		GEN_BSIFI_S(d, v, 29);					\
		GEN_BSIFI_S(d, v, 31);					\

#define GEN_BSIFI_SMALL(d, v)						\
		GEN_BSIFI_S(d, v, 2);					\
		GEN_BSIFI_S(d, v, 3);					\
		GEN_BSIFI_S(d, v, 5);					\
		GEN_BSIFI_S(d, v, 6);					\
		GEN_BSIFI_S(d, v, 13);					\
		GEN_BSIFI_S(d, v, 23);					\
		GEN_BSIFI_S(d, v, 27);					\
		GEN_BSIFI_S(d, v, 31);					\

#define GEN_BSEFI_S(v, w)						\
		CHECK_BSEFI(v, w, 0);					\
		CHECK_BSEFI(v, w, 1);					\
		CHECK_BSEFI(v, w, 2);					\
		CHECK_BSEFI(v, w, 3);					\
		CHECK_BSEFI(v, w, 4);					\
		CHECK_BSEFI(v, w, 5);					\
		CHECK_BSEFI(v, w, 6);					\
		CHECK_BSEFI(v, w, 7);					\
		CHECK_BSEFI(v, w, 8);					\
		CHECK_BSEFI(v, w, 9);					\
		CHECK_BSEFI(v, w, 10);					\
		CHECK_BSEFI(v, w, 11);					\
		CHECK_BSEFI(v, w, 12);					\
		CHECK_BSEFI(v, w, 13);					\
		CHECK_BSEFI(v, w, 14);					\
		CHECK_BSEFI(v, w, 15);					\
		CHECK_BSEFI(v, w, 16);					\
		CHECK_BSEFI(v, w, 17);					\
		CHECK_BSEFI(v, w, 18);					\
		CHECK_BSEFI(v, w, 19);					\
		CHECK_BSEFI(v, w, 20);					\
		CHECK_BSEFI(v, w, 21);					\
		CHECK_BSEFI(v, w, 22);					\
		CHECK_BSEFI(v, w, 23);					\
		CHECK_BSEFI(v, w, 24);					\
		CHECK_BSEFI(v, w, 25);					\
		CHECK_BSEFI(v, w, 26);					\
		CHECK_BSEFI(v, w, 27);					\
		CHECK_BSEFI(v, w, 28);					\
		CHECK_BSEFI(v, w, 29);					\
		CHECK_BSEFI(v, w, 31);					\

#define GEN_BSEFI_LARGE(v)						\
		GEN_BSEFI_S(v, 1);					\
		GEN_BSEFI_S(v, 2);					\
		GEN_BSEFI_S(v, 3);					\
		GEN_BSEFI_S(v, 4);					\
		GEN_BSEFI_S(v, 5);					\
		GEN_BSEFI_S(v, 6);					\
		GEN_BSEFI_S(v, 7);					\
		GEN_BSEFI_S(v, 8);					\
		GEN_BSEFI_S(v, 9);					\
		GEN_BSEFI_S(v, 10);					\
		GEN_BSEFI_S(v, 11);					\
		GEN_BSEFI_S(v, 12);					\
		GEN_BSEFI_S(v, 13);					\
		GEN_BSEFI_S(v, 14);					\
		GEN_BSEFI_S(v, 15);					\
		GEN_BSEFI_S(v, 16);					\
		GEN_BSEFI_S(v, 17);					\
		GEN_BSEFI_S(v, 18);					\
		GEN_BSEFI_S(v, 19);					\
		GEN_BSEFI_S(v, 20);					\
		GEN_BSEFI_S(v, 21);					\
		GEN_BSEFI_S(v, 22);					\
		GEN_BSEFI_S(v, 23);					\
		GEN_BSEFI_S(v, 24);					\
		GEN_BSEFI_S(v, 25);					\
		GEN_BSEFI_S(v, 26);					\
		GEN_BSEFI_S(v, 27);					\
		GEN_BSEFI_S(v, 28);					\
		GEN_BSEFI_S(v, 29);					\
		GEN_BSEFI_S(v, 31);					\

#define GEN_BSEFI_SMALL(v)						\
		GEN_BSEFI_S(v, 1);					\
		GEN_BSEFI_S(v, 3);					\
		GEN_BSEFI_S(v, 9);					\
		GEN_BSEFI_S(v, 13);					\
		GEN_BSEFI_S(v, 15);					\
		GEN_BSEFI_S(v, 17);					\
		GEN_BSEFI_S(v, 27);					\
		GEN_BSEFI_S(v, 31);					\

/* The large testsuite is too big for embedded ROM's.  */
static void check_bsifi_v(uint32_t d, uint32_t v)
{
	GEN_BSIFI_SMALL(d, v);
}

static void check_bsefi_v(uint32_t v)
{
	GEN_BSEFI_SMALL(v);
}

static void check_bsifi(void)
{
	unsigned int i;
	uint32_t d, v;

	printf("%s: ", __func__);
	for (i = 0; i < 1 * 1024; i++) {
		putchar('.');
		d = rand();
		v = rand();
		check_bsifi_v(d, v);
	}
	putchar('\n');
}

static void check_bsefi(void)
{
	unsigned int i;
	uint32_t v;

	printf("%s: ", __func__);
	for (i = 0; i < 1 * 1024; i++) {
		putchar('.');
		v = rand();
		check_bsefi_v(v);
	}
	putchar('\n');
}

void check_bitfield(void)
{
	printf("%s\n", __func__);

	check_bsifi();
	check_bsefi();
}
#endif /* MB_HAS_BITFIELD_INSNS */
