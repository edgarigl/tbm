#include <stdio.h>
#include "sys.h"

double g_x;
double *gp_x;

double _check_df_const_to_reg(void) __attribute__ ((noinline));
double _check_df_const_to_reg(void)
{
	return 0x123456789abcdefLL;
}

void check_df_const_to_reg(void) __attribute__ ((noinline));
void check_df_const_to_reg(void)
{
	if (_check_df_const_to_reg() != 0x123456789abcdefLL) {
		printf("%f\n", _check_df_const_to_reg());
		err();
	}
}

void check_df_const_to_mem(double *p) __attribute__ ((noinline));
void check_df_const_to_mem(double *p)
{
	*p = 0x123456789abeddeLL;
	if (g_x != 0x123456789abeddeLL) {
		printf("%f %f\n", g_x, *p);
		err();
	}
}

double _check_df_reg_to_reg(double a) __attribute__ ((noinline));
double _check_df_reg_to_reg(double a)
{
	return a;
}

double _check_df_reg_to_add_reg(double a, double b) __attribute__ ((noinline));
double _check_df_reg_to_add_reg(double a, double b)
{
	double r;
	printf("a=%x.%x\n", ((unsigned int *)&a)[0], ((unsigned int *)&a)[1]);
	printf("b=%x.%x\n", ((unsigned int *)&b)[0], ((unsigned int *)&b)[1]);
	r = a + b;
	printf("r=%x.%x sizeof=%ld\n", ((unsigned int *)&r)[0], ((unsigned int *)&r)[1], sizeof r);
	return r;
}

void check_df_reg_to_reg(void)
{
	long long r;
	printf("%s\n", __func__);
	if (_check_df_reg_to_reg(0xeddeeddeeddeeddeLL) != 0xeddeeddeeddeeddeLL) {
		err();
	}
	r = _check_df_reg_to_add_reg(0xeddeeddeeddeeddeLL, 0x1000000000000001LL);
	printf("r=%x.%x\n", ((unsigned int *)&r)[0], ((unsigned int *)&r)[1]);
	if (_check_df_reg_to_add_reg(0xeddeeddeeddeeddeLL, 0x1000000000000001LL) != 0xfddeeddeeddeeddfLL) {
		err();
	}
}

double _check_df_reg_plus_const_to_reg(double a) __attribute__ ((noinline));
double _check_df_reg_plus_const_to_reg(double a)
{
	return a + 0xffffffff;
}

void check_df_reg_plus_const_to_reg(void)
{
	printf("%s\n", __func__);
	if (_check_df_reg_plus_const_to_reg(0x1000000000000001LL) != 0x1000000100000000LL) {
		printf("%f\n", _check_df_reg_plus_const_to_reg(0x1000000000000001LL));
		err();
	}
}

void _check_df_reg_to_mem(double a) __attribute__ ((noinline));
void _check_df_reg_to_mem(double a)
{
	g_x = a;
}

void check_df_reg_to_mem(void)
{
	printf("%s\n", __func__);
	_check_df_reg_to_mem(0xdeadbeefeddebeefLL);
	if (g_x != 0xdeadbeefeddebeefLL)
		err();
}

void _check_df_mem_plus_mem_to_mem(double *d, double *a, double *b) __attribute__ ((noinline));
void _check_df_mem_plus_mem_to_mem(double *d, double *a, double *b)
{
	*d = *a + *b;
}

void check_df_mem_to_mem(void)
{
	double a = 0xeddeeddeeddeeddeLL, b = 0x1000000000000001LL;
	printf("%s\n", __func__);
	_check_df_mem_plus_mem_to_mem(&g_x, &a, &b);
	if (g_x != 0xfddeeddeeddeeddfLL)
		err();
}

double _check_extend_sf_df(float a) __attribute__ ((noinline));
double _check_extend_sf_df(float a)
{
        return a;
}

void check_df_extend_sf_df(void)
{
        if (_check_extend_sf_df(-1.0) != -1.0L)
                err();
}

void check_simple_float(void)
{
	float f = 3.00000;

	if (f != 3.00000)
		abort();
}

__attribute__ ((noinline))
double _check_div_df(double a, double b)
{
	return a / b;
}

__attribute__ ((noinline))
double _check_mul_df(double a, double b)
{
	return a * b;
}

__attribute__ ((noinline))
void check_df_mul(void)
{
	double x = -4;
	double r;

	printf("%s\n", __func__);
	r = _check_mul_df(x, x);
	if (r != 16) {
		err();
	}
	r = _check_mul_df(r, r);
	if (r != 256) {
		err();
	}
	r = _check_mul_df(r, r);
	if (r != 65536) {
		err();
	}
	r = _check_mul_df(r, r);
	if (r != 4294967296) {
		err();
	}
	r = _check_div_df(r, 65536);
	if (r != 65536) {
		err();
	}
}

void check_df(void)
{
        union {
                double d;
                unsigned int u32[2];
        } d;
        d.d = 0.123456789;

        printf("%s %x.%x\n", __func__, d.u32[0], d.u32[1]);

	/* Link the pointer.  */
	gp_x = &g_x;

	check_simple_float();
	check_df_mul();
	check_df_const_to_reg();
	check_df_const_to_mem(gp_x);
	check_df_reg_to_reg();
	check_df_reg_plus_const_to_reg();
	check_df_reg_to_mem();
	check_df_mem_to_mem();
	check_df_extend_sf_df();

	printf("%s DONE.\n", __func__);
}
