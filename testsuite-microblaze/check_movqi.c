#include "sys.h"

static volatile union {
	char b[4];
	short w[2];
	int  i;
} cmb = { .i = 0 };

void check_movqi(void)
{
#ifndef _LITTLE_ENDIAN
#warning "Port me to big endian."
#else
	puts(__func__);
#if 1
	cmb.i = 0x12345678;
	if (cmb.b[0] != 0x78) // || cmb != 0x99)
		err();
	if (cmb.b[1] != 0x56) // || cmb != 0x99)
		err();
#endif

	cmb.b[0] = 0x12;
	cmb.b[1] = 0x34;
	cmb.b[2] = 0x56;
	cmb.b[3] = 0x78;
#if 1
	if (cmb.i != 0x78563412)
		err();
#endif
#endif
	puts("movqi returns");
}
