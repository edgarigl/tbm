#ifndef _TESTSUITE_VEXPRESS_TRACE_H
#define _TESTSUITE_VEXPRESS_TRACE_H

#ifdef DEBUG
# include <stdio.h>
# define DPRINTF(fmt, ...) printf("%s: " fmt, __func__, ##__VA_ARGS__)
#else
# define DPRINTF(fmt, ...) do {} while(0)
#endif

#endif
