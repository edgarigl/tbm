#include "sys.h"

extern void check_movqi(void);
extern void check_movhi(void);

#if 1
void check_mov_const32(void) {
        volatile int x;
	puts(__func__);
        x = 0x1234567a;
        if (x != 0x1234567a)
                err();
}

volatile int cmm;
void check_mov_mem(void)
{
	puts(__func__);
	cmm = 32;
	if (cmm != 32)
		err();
	cmm = 0x1234567b;
	if (cmm != 0x1234567b)
		err();
}

volatile int cma[4];
void check_mov_array(void)
{
	puts(__func__);
	cma[0] = 0;
	cma[1] = 1;
	cma[2] = 2;
	cma[3] = 3;
	if (cma[0] != 0)
		err();
	if (cma[1] != 1)
		err();
	if (cma[2] != 2)
		err();
	if (cma[3] != 3)
		err();
}
#endif

#if 1
void check_mov(void)
{
	puts(__func__);

	check_mov_const32();
	check_mov_mem();
	check_mov_array();
	check_movqi();
	check_movhi();
}
#endif
