#include <stdio.h>
#include <string.h>
#include <time.h>
#include "testcalls.h"
#include "sys.h"

int check_c_sub(void);
void check_parity(void);
void check_cmp(void);
void check_mul(void);
void check_div(void);
void check_mov(void);
void check_mov(void);
void check_stackprot(void);
void check_clz(void);
void check_mbar(void);
void check_case(void);
void check_varargs(void);
void check_varargs2(void);
void check_ldst_rev(void);
void check_longlong(void);
void check_extend_sidi(void);
void check_df(void);

__attribute__ ((weak)) void check_bitfield(void)
{

}

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


	memcpy((void *) d, "nisse", 6);

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

void cpu_test(void)
{
	uint8_t version_code;
	uint32_t pvr0;
	int x;

	__asm__ __volatile__ ("mfs\t%0, rpvr0\n" : "=r" (pvr0));
	version_code = pvr0 >> 8;
	printf("pvr=%x\n", pvr0);

	printf("\n\nMicroBlaze v=%x %s endian CPU. " __DATE__ " " __TIME__ "\n",
			version_code, endian_s);

	check_bitfield();
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

	printf("ctest done at %lx\n", clock());
}

__testcall(cpu_test);
