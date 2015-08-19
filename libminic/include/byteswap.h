/*
 * Part of libminic.
 * Written by Edgar E. Iglesias.
 */
#ifndef __byteswap_h__
#define __byteswap_h__

static inline unsigned long long __bswap_64(unsigned long long x)
{
	return __builtin_bswap64(x);
}

static inline unsigned int __bswap_32(unsigned int x)
{
	return __builtin_bswap32(x);
}

static inline unsigned int __bswap_16(unsigned short x)
{
	unsigned short nx;

	nx = (x & 0xff) << 8;
	nx |= (x >> 8) & 0xff;
	/* No builtin?  */
	return nx;
}
#endif
