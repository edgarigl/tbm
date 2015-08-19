#include <stddef.h>

void * memcpy(void *pdst, const void *psrc, size_t pn);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *cs, const void *ct, size_t count);
void *memset(void *pdst, int c, size_t plen);
void *memchr(const void *s, int c, size_t n);

size_t strlen(const char *s);
char *strdup(const char *s);
char *strchr(const char *s, int c);
int strcmp(const char *cs, const char *ct);
