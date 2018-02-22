#ifndef _TESTSUITE_AARCH64_MISC_H
#define _TESTSUITE_AARCH64_MISC_H

static inline int ctz32(uint32_t val)
{
    return val ? __builtin_ctz(val) : 32;
}

#endif
