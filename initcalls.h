/*
 * Initcalls support.
 * Written by Edgar E. Iglesias
 */
#define __initcall(x) void (*__initcall_## x)(void) __attribute__((section (".initcall"), used)) = x
