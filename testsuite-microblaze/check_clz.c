/* Test cases for load reverse.  */
#include "sys.h"

static unsigned int ref_clz(unsigned int v)
{
    int cnt = 0;

    if (!(v & 0xffff0000U)) {
        cnt += 16;
        v <<= 16;
    }
    if (!(v & 0xff000000U)) {
        cnt += 8;
        v <<= 8;
    }
    if (!(v & 0xf0000000U)) {
        cnt += 4;
        v <<= 4;
    }
    if (!(v & 0xc0000000U)) {
        cnt += 2;
        v <<= 2;
    }
    if (!(v & 0x80000000U)) {
        cnt++;
        v <<= 1;
    }
    if (!(v & 0x80000000U)) {
        cnt++;
    }
    return cnt;
}

static unsigned int mb_ref_clz(unsigned int v)
{
	if (v == 0)
		return 32;
	if (v == ~0)
		return 0;

	return ref_clz(v);
}

static inline unsigned int mb_clz(unsigned int v)
{
	unsigned int r;
	__asm__ __volatile__ ("\tclz\t%0, %1\n" : "=r" (r) : "r" (v));
	return r;
}

void check_clz(void)
{
	unsigned int i, v;

	printf("%s\n", __func__);
	/* validate the reference and mb.  */
	if (mb_ref_clz(0) != 32)
		err();
	if (mb_ref_clz(0) != mb_clz(0))
		err();
	if (mb_ref_clz(~0) != 0)
		err();
	if (mb_ref_clz(~0) != mb_clz(~0))
		err();

	for (i = 0; i < 31; i++) {
		v = 1 << i;
		if (mb_ref_clz(v) != (31 - i))
			err();
		if (mb_ref_clz(v) != mb_clz(v))
			err();
	}
}
