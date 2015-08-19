/* Test cases for load reverse.  */
#include "sys.h"

#ifndef _LITTLE_ENDIAN
#define littleblaze 0
#define bigblaze 1
#else
#define littleblaze 1
#define bigblaze 0
#endif

#ifndef HAVE_LDST_REV
#warning "Warning you dont have ldst reverse!"
#endif

union {
	unsigned char d8[4];
	unsigned short d16[2];
	unsigned int d32;
} num1 = {0};

#if 1
void check_native_load(void)
{
	printf("%s\n", __func__);
	printf("num1.d32=%x\n", num1.d32);

	/* Check standard compiler emitted loads.  */
	if (littleblaze && num1.d32 != 0x78563412)
		err();
	if (bigblaze && num1.d32 != 0x12345678)
		err();

	printf("num1.d16[0]=%x\n", num1.d16[0]);
	if (littleblaze && num1.d16[0] != 0x3412)
		err();
	if (!littleblaze && num1.d16[0] != 0x1234)
		err();

	printf("num1.d16[1]=%x\n", num1.d16[1]);
	if (littleblaze && num1.d16[1] != 0x7856)
		err();
	if (!littleblaze && num1.d16[1] != 0x5678)
		err();

	printf("bigblaze=%d littleblaze=%d\n", bigblaze, littleblaze);
	printf("num1.d8[0]=%x\n", num1.d8[0]);
	printf("num1.d8[1]=%x\n", num1.d8[1]);
	printf("num1.d8[2]=%x\n", num1.d8[2]);
	printf("num1.d8[3]=%x\n", num1.d8[3]);
	/* If this one wails, something is really busted :)  */
	if (num1.d8[0] != 0x12
                || num1.d8[1] != 0x34
                || num1.d8[2] != 0x56
                || num1.d8[3] != 0x78)
		err();
}
#endif

#if HAVE_LDST_REV
#if 1
void check_reverse_load32(void)
{
	unsigned int w;

	printf("%s\n", __func__);
	asm ("lwr	%0, r0, %1\n" : "=r" (w) : "r" (&num1.d32));
	printf("Reverese num1.d32=%x\n", w);

	if (littleblaze && w != 0x12345678)
		err();
	if (!littleblaze && w != 0x78563412)
		err();
}
#endif

void check_reverse_load16(void)
{
	unsigned int w;
	volatile void *ptr;

#if 1
	ptr = &num1.d16[0];
	printf("%s %p\n", __func__, ptr);
	asm ("lhur	%0, r0, %1\n" : "=r" (w) : "r" (ptr));
	printf("Reverese num1.d16=%x\n", w);

	if (littleblaze && w != 0x5678)
		err();
	if (!littleblaze && w != 0x7856)
		err();
#endif
	ptr = &num1.d16[1];
	printf("%s ptr=%p\n", __func__, ptr);
	asm ("lhur	%0, r0, %1\n" : "=r" (w) : "r" (ptr));
	printf("Reverese num1.d16=%x\n", w);

	if (littleblaze && w != 0x1234)
		err();
	if (!littleblaze && w != 0x3412)
		err();
}

#if 1
void check_reverse_load8(void)
{
	unsigned int w;

	printf("%s\n", __func__);
	asm ("lbur	%0, r0, %1\n" : "=r" (w) : "r" (&num1.d8[0]));
	printf("Reverese num1.d8=%x\n", w);

	if (w != 0x78)
		err();

	printf("%s\n", __func__);
	asm ("lbur	%0, r0, %1\n" : "=r" (w) : "r" (&num1.d8[1]));
	printf("Reverese num1.d8=%x\n", w);

	if (w != 0x56)
		err();

	printf("%s\n", __func__);
	asm ("lbur	%0, r0, %1\n" : "=r" (w) : "r" (&num1.d8[2]));
	printf("Reverese num1.d8=%x\n", w);

	if (w != 0x34)
		err();

	printf("%s\n", __func__);
	asm ("lbur	%0, r0, %1\n" : "=r" (w) : "r" (&num1.d8[3]));
	printf("Reverese num1.d8=%x\n", w);

	if (w != 0x12)
		err();
}

void check_reverse_store32(void)
{
	unsigned int w;

	asm ("swr	%2, r0, %1 # %0\n" : "=m" (w) : "r" (&w), "r" (0x12345678));
	printf("w=%x\n", w);
	if (w != 0x78563412) {
		err();
	}
}

void check_reverse_store16(void)
{
	union {
		unsigned int w;
		unsigned short d16[2];
	} num;

	num.w = 0x0000edde;

	asm ("shr	%2, r0, %1 # %0\n" : "=m" (num.d16[1]) : "r" (&num.d16[0]), "r" (0x1234));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0x3412edde) {
		err();
	}
	if (bigblaze && num.w != 0x3412) {
		err();
	}

	num.w = 0xedde0000;
	asm ("shr	%2, r0, %1 # %0\n" : "=m" (num.d16[0]) : "r" (&num.d16[1]), "r" (0x1234));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0xedde3412) {
		err();
	}
	if (bigblaze && num.w != 0x34120000) {
		err();
	}
}

void check_reverse_store8(void)
{
	union {
		unsigned int w;
		unsigned char d8[4];
	} num;

	num.w = 0xeddeedde;

	asm ("sbr	%2, r0, %1 # %0\n" : "=m" (num.d8[3]) : "r" (&num.d8[0]), "r" (0xFF));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0xFFdeedde) {
		err();
	}
	if (bigblaze && num.w != 0xeddeedFF) {
		err();
	}

	num.w = 0xeddeedde;
	asm ("sbr	%2, r0, %1 # %0\n" : "=m" (num.d8[2]) : "r" (&num.d8[1]), "r" (0xFF));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0xedFFedde) {
		err();
	}
	if (bigblaze && num.w != 0xeddeFFde) {
		err();
	}

	num.w = 0xeddeedde;
	asm ("sbr	%2, r0, %1 # %0\n" : "=m" (num.d8[1]) : "r" (&num.d8[2]), "r" (0xFF));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0xeddeFFde) {
		err();
	}
	if (bigblaze && num.w != 0xedFFedde) {
		err();
	}

	num.w = 0xeddeedde;
	asm ("sbr	%2, r0, %1 # %0\n" : "=m" (num.d8[0]) : "r" (&num.d8[3]), "r" (0xFF));
	printf("w=%x\n", num.w);
	if (littleblaze && num.w != 0xeddeedFF) {
		err();
	}
	if (bigblaze && num.w != 0xFFdeedde) {
		err();
	}

}
#endif
#endif

void check_ldst_rev(void)
{
	num1.d8[0] = 0x12;
	num1.d8[1] = 0x34;
	num1.d8[2] = 0x56;
	num1.d8[3] = 0x78;

	check_reverse_load16();
#if 1
	check_native_load();
#if HAVE_LDST_REV
	check_reverse_load32();
	check_reverse_load16();
	check_reverse_load8();
	check_reverse_store32();
	check_reverse_store16();
	check_reverse_store8();
#endif
#endif
}
