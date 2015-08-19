/*
Copyright (c) 2009, Edgar E. Iglesias <edgar.iglesias@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met: 

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies, 
either expressed or implied, of the FreeBSD Project.
*/

/*
 * Minimalistic printf.
 *
 * Written by Edgar E. iglesias.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

typedef int (*putchar_f)(void *ctx, int c);

extern int putchar(int c);

static int real_putchar(void *ctx, int c) {
	return putchar(c);
}

static int null_putchar(void *ctx, int c) {
	return c;
}

static int dec_out(putchar_f f, void *ctx, uint64_t val)
{
	unsigned char d[32];
	unsigned int i = 0, r;
	uint64_t nval;

	while (val) {
		nval = val / 10;
		d[i++] = '0' + val - (nval * 10);
		val = nval;
	}

	if (i == 0)
		d[i++] = '0';

	r = i;
	while (i)
		f(ctx, d[--i]);
	return r;
}

static inline int hexchar(unsigned int val)
{
	val &= 0xf;
	if (val > 9)
		return val - 10 + 'a';
	return val + '0';
}

static int hex_out(putchar_f f, void *ctx, uint64_t val)
{
	unsigned char d[32];
	unsigned int i = 0, r;

	while (val) {
		d[i++] = hexchar(val);
		val >>= 4;
	}
	if (i == 0)
		d[i++] = '0';

	r = i;
	while (i)
		f(ctx, d[--i]);
	return r;
}

static int str_out(putchar_f f, void *ctx, char *s)
{
	int r = 0;
	while (s && *s) {
		f(ctx, *s++);
		r++;
	}
	return r;
}

static int _vprintf_f(putchar_f f, void *ctx, const char *fmt, va_list va)
{
	char c;
	int r = 0;
	int escaped = 0;
	unsigned int longs = 0;
	uint64_t v;

	while ((c = (*fmt++))) {
		if (escaped) {
			switch(c) {
				case '%':
					f(ctx, c);
					r++;
					break;
				case 'p':
					r += hex_out(f, ctx, (uintptr_t) va_arg(va, void *));
					break;
				case 'u':
				case 'd':
				case 'x':
					switch (longs) {
					default:
						v = va_arg(va, unsigned int);
						break;
					case 1:
						v = va_arg(va, long unsigned int);
						break;
					case 2:
						v = va_arg(va, long long unsigned int);
						break;
					}
					if (c == 'x') {
						r += hex_out(f, ctx, v);
					} else {
						r += dec_out(f, ctx, v);
					}
					break;
				case 'c':
					f(ctx, va_arg(va, int));
					r += 1;
					break;
				case 's':
					r += str_out(f, ctx, va_arg(va, char *));
					break;
				case 'z':
					continue;
				case 'l':
					longs++;
					continue;
				default:
					break;
			}
			escaped = 0;
			longs = 0;
		} else {
			if (c == '%') {
				escaped = 1;
			}
			else
				f(ctx, c);
		}
	}
	return r;
}

int vprintf(const char *fmt, va_list va)
{
	return _vprintf_f(real_putchar, NULL, fmt, va);
}

struct vap_ctx {
	char *str;
	size_t pos;
};

int vap_putchar(void *opaque, int c)
{
	struct vap_ctx *ctx = opaque;
	ctx->str[ctx->pos++] = c;
	return c;
}

int vasprintf(char **strp, const char *fmt, va_list va)
{
	struct vap_ctx ctx;
	int size;

	size = _vprintf_f(null_putchar, NULL, fmt, va);
	if (size <= 0)
		return size;

	ctx.str = *strp = malloc(size + 1);
	ctx.pos = 0;
	return _vprintf_f(vap_putchar, &ctx, fmt, va);
}

