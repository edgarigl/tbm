#ifndef __SYS_H__
#define __SYS_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void hexdump(const char *prefix, const void *buf, size_t len);

#include "arch-sys.h"

#include "unitname.h"

#define typeof __typeof__
#define asm __asm__

extern void arch_init(void);
extern void app_run(void);

void sys_dump_regs(struct excp_frame *f);
void panic(const char *str);

/* Legacy from C3P3U.  */
static inline void puthex(unsigned int v)
{
	printf("%x", v);
}

void udelay(unsigned int us);

/* Compiler barrier to avoid reordering across this.  */
#define barrier() __asm__ __volatile__ ("" : : : "memory");

#include "io.h"

#define err() { printf("ERROR: %s:%d\n", __FILE__, __LINE__); \
		exit(1); }
#define abort err


#ifndef glue
#define xglue(x, y)     x ## y
#define glue(x, y)      xglue(x, y)
#endif
#define stringify(s)    tostring(s)
#define tostring(s)     #s

#define ZBUILDERR(x)    (sizeof (char [1 - 2 * !!(x)]) - 1)
#define IS_ARRAY(x)     (!__builtin_types_compatible_p(typeof(x), \
                                                        typeof(&x[0])))
#define ARRAY_SIZE(x)   (sizeof (x) / sizeof (*x) + ZBUILDERR(!IS_ARRAY(x)))
#endif
