/*
 * This is part of libminic
 * Written by Edgar E. Iglesias.
 */
#ifndef __stdio_h__
#define __stdio_h__

#include <stddef.h>

#define EOF (-1)

typedef int FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int fputc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

#include <stdarg.h>

int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int vprintf(const char *fmt, va_list va);
int vasprintf(char **strp, const char *fmt, va_list va);
#endif
