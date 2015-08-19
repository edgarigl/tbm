#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* memPcpy version for fully aligned copies.  */
void *fully_aligned32_mempcpy(void *d, const void *s, size_t n)
{
	uint32_t *d32 = d;
	const uint32_t *s32 = s;

	while (n) {
		*d32 = *s32;
		d32++;
		s32++;
		n -= 4;
	}
	return d32;
}

void *memcpy(void *d, const void *s, size_t n)
{
	char l;
	unsigned char *bd = d;
	const unsigned char *bs = s;
	uintptr_t pd = (unsigned long) d;
	uintptr_t ps = (unsigned long) s;

	if ((pd & 3) == 0 && (ps & 3) == 0) {
		size_t n_aligned = n & ~3;

		bd = fully_aligned32_mempcpy(d, s, n_aligned);
		if ((n & 3) == 0)
			return d;

		/* Fix up the last chunk.  */
		bs += n_aligned;
		n -= n_aligned;
	}

	while (n--) {
		l = *bs++;
		*bd++ = l;
	}
	return d;
}
