#define stdin ignore_stdin
#define stdout ignore_stdout
#define stderr ignore_stderr
#include <stdio.h>

#undef stderr
#undef stdout
#undef stdin
const FILE __attribute__ ((weak)) * const stderr = (void *) 2;
const FILE __attribute__ ((weak)) * const stdout = (void *) 1;
const FILE __attribute__ ((weak)) * const stdin = (void *) 0;
