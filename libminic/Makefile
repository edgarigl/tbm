#CROSS	= c3p3u-none-elf-
#CROSS	= scpu-unknown-elf-newlib-
#CROSS  = arm-linux-uclibc-
#CROSS	= mb-linux-
#CROSS	= mipsel-unknown-linux-gnu-
#CROSS	= /usr/local/cris/r64/bin/crisv32-axis-linux-gnu-
#CROSS	= /usr/local/cris/r64/bin/cris-axis-linux-gnu-

#CROSS   ?= crisv32-axis-linux-gnu-
#CROSS  = cris-axis-linux-gnu-
#CROSSROOT=/home/edgar/cross-cris/v10
#CFLAGS  += -march=v10 -mno-mul-bug-workaround

ifeq ($V,)
QUIET=@
quietprint=@echo "  $(1)"
.SILENT:
else
QUIET=
quietprint=
endif

SRCDIR =
BUILDDIR ?=

CC      = $(CROSS)gcc
AS      = $(CROSS)gcc -x assembler-with-cpp -c
LD      = $(CC)
SIZE	= $(CROSS)size

CFLAGS  += $(CPUFLAG)
CFLAGS  += -Wall -g -O2 -fomit-frame-pointer -ftest-coverage
CFLAGS  += -Werror
# Avoid having GCC identify loop patterns and emitting calls
# to memset, memcpy and friends. If not, GCC inserts a call
# to memset from within memset itself, causing endless
# recursion.
CFLAGS  += -fno-tree-loop-distribute-patterns
CPPFLAGS += -isystem include -nostdinc

S_OBJS   = malloc.o ctype.o sort.o vprintf.o assert.o rand.o
S_OBJS  += memmove.o

S_OBJS  += stdio/stdio.o
S_OBJS  += stdio/fopen.o
S_OBJS  += stdio/fclose.o
S_OBJS  += stdio/puts.o
S_OBJS  += stdio/putchar.o
S_OBJS  += stdio/fputc.o
S_OBJS  += stdio/fread.o
S_OBJS  += stdio/fwrite.o
S_OBJS  += stdio/fprintf.o
S_OBJS  += stdio/printf.o
S_OBJS  += stdio/setvbuf.o

S_OBJS  += unistd/exit.o
S_OBJS  += unistd/open.o
S_OBJS  += unistd/read.o
S_OBJS  += unistd/write.o
S_OBJS  += unistd/close.o
S_OBJS  += unistd/stat.o
S_OBJS  += unistd/fstat.o
S_OBJS  += unistd/mmap.o
S_OBJS  += unistd/mmap2.o

S_OBJS  += signal/raise.o

S_OBJS  += time/clock.o

LIBC_ARCH ?= generic

S_OBJS  += arch-$(LIBC_ARCH)/memset.o
S_OBJS  += arch-$(LIBC_ARCH)/memcmp.o
S_OBJS  += arch-$(LIBC_ARCH)/memchr.o
S_OBJS  += arch-$(LIBC_ARCH)/memcpy.o
S_OBJS  += arch-$(LIBC_ARCH)/strcmp.o
S_OBJS  += arch-$(LIBC_ARCH)/strlen.o
S_OBJS  += arch-$(LIBC_ARCH)/strchr.o
S_OBJS  += arch-$(LIBC_ARCH)/strdup.o

#S_OBJS  += string.o
OBJS = $(addprefix $(BUILDDIR), $(S_OBJS))

BUILD_SUBDIRS += stdio
BUILD_SUBDIRS += unistd
BUILD_SUBDIRS += signal
BUILD_SUBDIRS += time
BUILD_SUBDIRS += arch-$(LIBC_ARCH)

TARGET = $(BUILDDIR)libminic.a

# Create the dirs.
CREATE_BUILDDIRS = $(foreach d, $(BUILD_SUBDIRS),                       \
                        $(shell mkdir -p $(BUILDDIR)$(d)))

all: $(CREATE_BUILDDIRS) $(TARGET)

-include $(abspath $(OBJS:.o=.d))
CFLAGS += -MMD

$(TARGET): $(OBJS)
	$(call quietprint, $@)
	$(QUIET)$(AR) ucr $@ $^

$(BUILDDIR)%.o: $(SRCDIR)%.c
	$(call quietprint, $<)
	$(QUIET)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

$(BUILDDIR)%.o: $(SRCDIR)%.S
	$(call quietprint, $<)
	$(QUIET)$(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

install: $(TARGET)
	mkdir -p $(prefix)/lib
	mkdir -p $(prefix)/include
	cp inttypes.h $(prefix)/include
	cp libminic.a $(prefix)/lib

size: $(TARGET)
	$(SIZE)	$(TARGET)

clean:
	$(MAKE) -C testsuite clean
	rm -fr $(TARGET) $(OBJS) $(OBJS:.o=.d)
