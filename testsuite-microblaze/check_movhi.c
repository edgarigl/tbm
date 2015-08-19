#include "sys.h"

static volatile union {
	char b[4];
	short w[2];
	int  i;
} cmb = { .i = 0 };

void check_movhi(void)
{
#ifndef _LITTLE_ENDIAN
#warning "Port me to big endian."
#else
	puts(__func__);
#if 1
	cmb.i = 0x12345678;
	if (cmb.w[0] != 0x5678)
		err();
	if (cmb.w[1] != 0x1234)
		err();
#endif
#if 1
	cmb.w[0] = 0x7856;
	cmb.w[1] = 0x3412;
	if (cmb.i != 0x34127856)
		err();
#endif
#endif
}
