#include "sys.h"

static unsigned long long _bug(unsigned long long q, unsigned long tmp)
                        __attribute__ ((noinline));
static unsigned long long _bug(unsigned long long q, unsigned long tmp)
{
        return q = (unsigned long long) q * tmp;
}

static void bug(void)
{
        unsigned long long r;
        r = _bug(1ULL, 0x56b811c);
        printf("%llx\n", r);
}

void check_extend_sidi(void)
{
        bug();
}

