#include <stdio.h>
#include "testcalls.h"
#include "sys.h"

extern int check_c_sub(void);
extern void check_parity(void);
extern void check_cmp(void);
extern void check_mul(void);
extern void check_div(void);
extern void check_mov(void);

void check_simple_arg(int a) {
	puts(__func__);
        if (a)
                err();
}

void check_c_lsl(void) {
        volatile int x;
	puts(__func__);
        x  = 100;
	x <<= 3;
        if (x != 800)
                err();
}

void check_c_and(void) {
        volatile int x;
	puts(__func__);
        x  = 0x111;
        x &= 0x101;
        if (x != 0x101)
                err();
}

void check_c_xor(void) {
	volatile int x;
	puts(__func__);
	x  = 0x111;
	x ^= 0x101;
	if (x != 0x010)
		err();
}

#define LOOPS 30
static volatile int myarr[LOOPS];
void check_for(void) {
	volatile int i;

	puts(__func__);
	for (i = 0; i < LOOPS; i++) {
		myarr[i] = i + 1;
	}
}

void check_memcpy(void)
{
	volatile char d[10] = "kalle";

	puts(__func__);
	if (d[0] != 'k')
		err();
	if (d[1] != 'a')
		err();
	if (d[2] != 'l')
		err();
	if (d[3] != 'l')
		err();
	if (d[4] != 'e')
		err();
	if (d[5] != '\0')
		err();


	memcpy(d, "nisse", 6);

	if (d[0] != 'n')
		err();
	if (d[1] != 'i')
		err();
	if (d[2] != 's')
		err();
	if (d[3] != 's')
		err();
	if (d[4] != 'e')
		err();
	if (d[5] != '\0')
		err();

}

static int check_ret(void) __attribute__ ((noinline));
static int check_ret(void)
{
	return 0xaaaabbbb;
}

unsigned int tsta[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

#ifdef _LITTLE_ENDIAN
const char endian_s[] = "Little";
#else
const char endian_s[] = "Big";
#endif

int cpu_test(int argc, char **argv)
{
	int x;

	printf("\n\nMicroBlaze %s endian CPU. " __DATE__ " " __TIME__ "\n", 
			endian_s);

/*	petasupport_test_loop_local(); */
#if 1
#if 1
	check_stackprot();
	check_clz();
	check_mbar();
	check_for();
	check_memcpy();
	x = check_ret();
	if (x != (int)0xaaaabbbb)
		err();
	check_simple_arg(0);

	check_c_sub();
	check_cmp();
	check_mov();
	check_c_lsl();
	check_c_and();
	check_c_xor();
	check_for();
	check_case();
	check_varargs();
	check_varargs2();
	check_parity();
	check_mul();
	check_div();
	check_ldst_rev();
	check_longlong();
	check_extend_sidi();
	check_df();
	check_gcc_930603_1();
#endif
#endif

	printf("ctest done at %x\n", clock());
	return 0;
}

__testcall(cpu_test);
