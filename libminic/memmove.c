#include <string.h>

#ifdef TEST_GNU_BCOPY
void bcopy (const void *src, void *dest, size_t n)
#else
void bcopy (const char *src, char *dest, size_t n)
#endif
{
	const unsigned char *src_u8 = (unsigned char*) src;
	unsigned char *dst_u8 = (unsigned char*) dest;

	if (dst_u8 < src_u8) {
		while (n--)
			*dst_u8++ = *src_u8++;
	} else {
		const unsigned char *lasts = src_u8 + (n-1);
		unsigned char *lastd = dst_u8 + (n-1);
		while (n--)
			*lastd-- = *lasts--;
	}
}

void *memmove(void *dest, const void *src, size_t n)
{
	bcopy(src, dest, n);
	return dest;
}
