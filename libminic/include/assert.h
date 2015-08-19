/*
 * This is part of libminic.
 * Written by Edgar E. Iglesias
 */
#ifndef __assert_h__
#define __assert_h__

#ifdef NDEBUG
#define assert(unused) ((void)0)
#else
#ifdef TINY_ASSERT
void __assert(const char *str);
#define assert(exp) ((exp) ? (void) 0 : __assert(__FILE__ ": assert: " #exp))
#else
void __assert(const char *str, unsigned int line);
#define assert(exp) ((exp) ? (void) 0 : \
			__assert(__FILE__ ":%d: assert: " #exp "\n", __LINE__))
#endif
#endif

#endif
