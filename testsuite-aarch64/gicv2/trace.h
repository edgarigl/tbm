#ifndef _TESTSUITE_VEXPRESS_TRACE_H
#define _TESTSUITE_VEXPRESS_TRACE_H

#ifdef DEBUG
# include <stdio.h>
# define DPRINTF(fmt, ...) do { \
    const int PADDING = 18; \
    int rest = PADDING - ((int)sizeof(__func__)) + 2; \
    while (rest-- > 0) { putchar(' '); } \
    printf("[%s] " fmt, __func__, ##__VA_ARGS__); \
} while (0)
#else
# define DPRINTF(fmt, ...) do {} while(0)
#endif

#endif
