#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "sys.h"
#include "testcalls.h"

static void showb(int b)
{
	printf("b=%d\n", b);
}

static void showa(int a)
{
	printf("a=%d\n", a);
}

void dummy_func(int a, void (*func)(int a), void (*func2)(int a))
{
	func(a);
}

void dummy_func2(int a, void (*func)(int a), void (*func2)(int a))
{
	func2(a);
}

#define UART_BASEADDR ((char *) 0x84000000)
#define R_ADDR (4 * 4)
#define R_VAL (5 * 4)
#define R_WRITE (6 * 4)
#define R_RAMCFG (7 * 4)

static void check_exec(void)
{
	uint32_t *p1 = (void *) 0x80000;
	uint32_t *p2 = (void *) 0xa0000;
	void (*execptr)(int a, void (*func)(int a), void (*func2)(int a));
	unsigned cpuid;
	printf("%s\n", __func__);

	__asm__ __volatile__ ("mfs %0, rpvr1" : "=r" (cpuid));
	printf("cpuid %d\n", cpuid);

	if (cpuid == 0) {
		execptr = (void *) p1;
	} else {
		execptr = (void *) p2;
	}
	memcpy(execptr, dummy_func, 128);
	execptr(cpuid, showa, showb);
	memcpy(execptr, dummy_func2, 128);
	execptr(cpuid, showa, showb);

	writel(UART_BASEADDR + R_ADDR, (unsigned long) execptr);
	writel(UART_BASEADDR + R_VAL, 0xdeadbeef);
	writel(UART_BASEADDR + R_WRITE, 1);

	if (cpuid != 0) {
		printf("change the mapping\n");
		writel(UART_BASEADDR + R_RAMCFG, 0);
	}
}

static void check_mem(void)
{
	volatile uint32_t *p1 = (void *) 0x60000;
	volatile uint32_t *p2 = (void *) 0x40000;
	unsigned cpuid;
	printf("%s\n", __func__);

	__asm__ __volatile__ ("mfs %0, rpvr1" : "=r" (cpuid));
	printf("cpuid %d\n", cpuid);

	printf("p1 = %x\n", *p1);
	printf("p2 = %x\n", *p2);
	*p1 = 0xedde0 + cpuid;

	if (cpuid == 0) {
		assert(*p1 == 0x0);
		assert(*p2 == 0);
	} else {
		assert(*p1 == 0xedde0 + cpuid);
		assert(*p2 == 0xedde0 + cpuid);
	}

	*p2 = 0xedde00 + cpuid;
	if (cpuid == 0) {
		assert(*p1 == 0);
		assert(*p2 == 0xedde00 + cpuid);
	} else {
		assert(*p1 == 0xedde00 + cpuid);
		assert(*p2 == 0xedde00 + cpuid);
	}

	printf("p1 = %x\n", *p1);
	printf("p2 = %x\n", *p2);
}

static void check_aspace(void)
{
	check_mem();
	check_exec();
}

__testcall(check_aspace);
