/*
 * Testcalls support.
 * Written by Edgar E. Iglesias
 */
#define __testcall(x) void (*__testcall_## x)(void) __attribute__((section (".testcall"), used)) = x

int __testcalls_exec(void);
