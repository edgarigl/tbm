#ifndef __unistd_h__
#define __unistd_h__

#include <stddef.h>
#include <sys/stat.h>

/* Most of these are from when libminic could run on top of
   linux.  */

/* Hax for debug.  */
extern unsigned int write_to_debug_reg;
int write(int fd, const void *buf, size_t count);

int read(int fd, void *buf, size_t count);

int stat(const char *path, struct stat *buf);
int __xstat(int ver, const char *path, struct stat *buf);

int fstat(int fd, struct stat *buf);
int __fxstat(int ver, int fd, struct stat *buf);

int close(int fd);
void exit (int status);

#endif
