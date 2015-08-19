/*
 * Part of libminic.
 * Written by Edgar E. Iglesias.
 */
#ifndef __arpa_inet_h__
#define __arpa_inet_h__

#include <endian.h>
#include <byteswap.h>

static inline unsigned int htonl(unsigned int x)
{
# if __BYTE_ORDER__ == __LITTLE_ENDIAN
	x = __bswap_32(x);
#endif
        return x;
}
static inline unsigned short htons(unsigned short x)
{
# if __BYTE_ORDER__ == __LITTLE_ENDIAN
	x = __bswap_16(x);
#endif
        return x;
}
#endif
