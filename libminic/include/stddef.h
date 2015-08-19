#ifndef __stddef_h__
#define __stddef_h__

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned long size_t;
typedef long ssize_t;

#define offsetof(st, m) __builtin_offsetof(st, m)

#endif
