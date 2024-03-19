#include <stdio.h>
#include "sys.h"

static long long g_x;
static long long *gp_x;

#if 1
long long _check_ll_const_to_reg(void) __attribute__ ((noinline));
long long _check_ll_const_to_reg(void)
{
	return 0x123456789abcdefLL;
}

void check_ll_const_to_reg(void) __attribute__ ((noinline));
void check_ll_const_to_reg(void)
{
	if (_check_ll_const_to_reg() != 0x123456789abcdefLL) {
		printf("%llx\n", _check_ll_const_to_reg());
		err();
	}
}

void check_ll_const_to_mem(long long *p) __attribute__ ((noinline));
void check_ll_const_to_mem(long long *p)
{
	*p = 0x123456789abeddeLL;
	if (g_x != 0x123456789abeddeLL) {
		printf("%llx %llx\n", g_x, *p);
		err();
	}
}

long long _check_ll_reg_to_reg(long long a) __attribute__ ((noinline));
long long _check_ll_reg_to_reg(long long a)
{
	return a;
}

long long _check_ll_reg_to_add_reg(long long a, long long b) __attribute__ ((noinline));
long long _check_ll_reg_to_add_reg(long long a, long long b)
{
	return a + b;
}

void check_ll_reg_to_reg(void)
{
	printf("%s\n", __func__);
	if (_check_ll_reg_to_reg(0xeddeeddeeddeeddeLL) != 0xeddeeddeeddeeddeLL) {
		err();
	}
	if (_check_ll_reg_to_add_reg(0xeddeeddeeddeeddeLL, 0x1000000000000001LL) != 0xfddeeddeeddeeddfLL) {
		err();
	}
}

__attribute__ ((noinline))
long long _check_ll_reg_plus_const_to_reg(long long a)
{
	return a + 0xffffffff;
}

void check_ll_reg_plus_const_to_reg(void)
{
	printf("%s\n", __func__);
	if (_check_ll_reg_plus_const_to_reg(0x1000000000000001LL) != 0x1000000100000000LL) {
		printf("%llx\n", _check_ll_reg_plus_const_to_reg(0x1000000000000001LL));
		err();
	}
}

__attribute__ ((noinline))
void _check_ll_reg_to_mem(long long a)
{
	g_x = a;
}

void check_ll_reg_to_mem(void)
{
	printf("%s\n", __func__);
	_check_ll_reg_to_mem(0xdeadbeefeddebeefLL);
	if (g_x != 0xdeadbeefeddebeefLL)
		err();
}

void _check_ll_mem_plus_mem_to_mem(long long *d, long long *a, long long *b) __attribute__ ((noinline));
void _check_ll_mem_plus_mem_to_mem(long long *d, long long *a, long long *b)
{
	*d = *a + *b;
}

void check_ll_mem_to_mem(void)
{
	long long a = 0xeddeeddeeddeeddeLL, b = 0x1000000000000001LL;
	printf("%s\n", __func__);
	_check_ll_mem_plus_mem_to_mem(&g_x, &a, &b);
	if (g_x != 0xfddeeddeeddeeddfLL)
		err();
}

__attribute__((noinline)) long long _check_extend_si_di(long a)
{
	return a;
}

void check_ll_extend_si_di(void)
{
	long long r;

	r = _check_extend_si_di(0x80000000);
	printf("r=%llx\n", r);
	if (_check_extend_si_di(0xffffffff) != 0xffffffffffffffffLL)
		err();
	if (_check_extend_si_di(0x80000000) != 0xffffffff80000000LL)
		err();
	if (_check_extend_si_di(0x7fffffff) != 0x7fffffffLL)
		err();
}
#endif

__attribute__ ((noinline))
long long _check_ll_mod_reg(long long a, long m)
{
	return a % m;
}

void check_ll_mod_reg(void)
{
	if (_check_ll_mod_reg(0x123456789abcdefLL, 17) != 8)
		err();
}

#if 1
__attribute__ ((noinline))
long long _check_ll_mul(long long a, long long b)
{
	return a *b;
}
void check_ll_mul(void)
{
	printf("%s:\n", __func__);
	if (_check_ll_mul(0xeddeeddeeddeLL, 0x2LL) != 0x1dbbddbbddbbcLL) {
		err();
	}
	if (_check_ll_mul(-16LL, 0x2LL) != -32LL) {
		err();
	}
}
#endif
void check_longlong(void)
{
	printf("%s\n", __func__);

	/* Link the pointer.  */
	gp_x = &g_x;

#if 1
	check_ll_extend_si_di();
	check_ll_const_to_reg();
	check_ll_const_to_mem(gp_x);
	check_ll_reg_to_reg();
	check_ll_reg_plus_const_to_reg();
	check_ll_reg_to_mem();
	check_ll_mem_to_mem();
#endif
//	check_ll_mod_reg();
	check_ll_mul();

	printf("%s DONE.\n", __func__);
}
