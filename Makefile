#
# TBM Makefile build system.
#
# Copyright (C) 2009 - 2014 Edgar E. Iglesias <edgar.iglesias@gmail.com>
# Written by Edgar E. Iglesias
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The views and conclusions contained in the software and documentation are those
# of the authors and should not be interpreted as representing official policies,
# either expressed or implied, of the FreeBSD Project.
#

# Default to microblaze but user can override on cmd line.

# Give CFG the oportunity to override this rule.

ifneq "$(CFG)" ""
include $(CFG)
endif

-include .config

ARCH ?= microblaze
BOARD ?= ronaldo
UNITNAME ?= $(ARCH)
TESTSUITE_ARCH ?= testsuite-$(ARCH)
TESTSUITE_BOARD ?= testsuite-$(BOARD)
PLAT_DIR ?= plat/$(BOARD)
DRIVER_BOARD ?= drivers-$(BOARD)
APPDIR ?= app/$(BOARD)/$(UNITNAME)$(UNIT_TNAME)

ifneq "$(ARCH)" "none"
include arch-$(ARCH)/arch.mk
endif

SRCDIR =
BUILD_ROOT = build
BUILDDIR = $(BUILD_ROOT)/$(BOARD)/$(UNITNAME)$(UNIT_TNAME)/

CC      = $(CROSS)gcc
CXX     = $(CROSS)g++
AS      = $(CC)
LD      = $(CROSS)gcc
SIZE	= $(CROSS)size
OBJCOPY	= $(CROSS)objcopy
OBJDUMP	= $(CROSS)objdump

BOARD_ = $(subst -,_,$(BOARD))

CFLAGS  += -Wall -g3 -O2 -nostdlib -I mb-testsuite
CFLAGS  += -ffunction-sections -fdata-sections
CFLAGS  += -ftest-coverage
#CFLAGS  += -Werror
CPPFLAGS += -D__BOARD__="$(BOARD_)" -D__UNIT__="$(UNITNAME)"
CPPFLAGS += -D__$(BOARD_)__ -D__$(UNITNAME)__
CPPFLAGS += -nostdinc -I $(CURDIR)
CPPFLAGS += -I arch-$(ARCH)/
CPPFLAGS += -I $(CURDIR)/libminic/include
ASFLAGS += -D__ASSEMBLY__
ASFLAGS += -g

#LDFLAGS += -Wl,--no-relax
LDFLAGS += -Wl,-relax
LDFLAGS += -nostdlib
LDFLAGS += -Wl,--gc-sections

LDFLAGS += -L $(BUILDDIR)/configs/
LDFLAGS += -L $(BUILDDIR)libminic
LDFLAGS += -T$(LDSCRIPT)
LDLIBS   = -Wl,--start-group -lminic -lgcc -Wl,--end-group #-lstdc++

ifneq "$(ARCH)" "none"
ARCH_SRCS = $(shell find arch-$(ARCH)/ -maxdepth 1 -name \*.c)
ARCH_OBJS = $(ARCH_SRCS:.c=.o)
BUILD_SUBDIRS += arch-$(ARCH)

TEST_ARCH_SRCS = $(shell [ -d $(TESTSUITE_ARCH) ] && find $(TESTSUITE_ARCH)/ -maxdepth 1 -name \*.c)
TEST_ARCH_OBJS = $(TEST_ARCH_SRCS:.c=.o)
BUILD_SUBDIRS += $(TESTSUITE_ARCH)
endif

BOARD_SRCS = $(shell [ -d $(TESTSUITE_BOARD) ] && find $(TESTSUITE_BOARD)/ -maxdepth 1 -name \*.c)
BOARD_SRCS += $(shell [ -d $(PLAT_DIR) ] && find $(PLAT_DIR)/ -maxdepth 1 -name \*.c)
BOARD_OBJS = $(BOARD_SRCS:.c=.o)

DRIVER_SRCS ?= $(shell [ -d $(DRIVER_BOARD) ] && find $(DRIVER_BOARD)/ -maxdepth 1 -name \*.c)
DRIVER_OBJS = $(DRIVER_SRCS:.c=.o)

APP_SRCS = $(shell [ -d $(APPDIR) ] && find $(APPDIR)/ -maxdepth 1 -name \*.c)
APP_OBJS = $(APP_SRCS:.c=.o)
CPPFLAGS += -I $(APPDIR)/

LIBVERIF_DIR = libverif
LIBVERIF_SRCS = $(shell find $(LIBVERIF_DIR)/ -maxdepth 1 -name \*.c)
LIBVERIF_OBJS = $(LIBVERIF_SRCS:.c=.o)

EHASH_DIR = ehash
EHASH_SRCS = $(shell find $(EHASH_DIR)/ -maxdepth 1 -name \*.c)
EHASH_OBJS = $(EHASH_SRCS:.c=.o)
CPPFLAGS += -I $(CURDIR)/ehash/

LIBFDT_DIR = libfdt
include $(LIBFDT_DIR)/Makefile.libfdt
TBM_LIBFDT_SRCS = $(addprefix $(LIBFDT_DIR)/, $(LIBFDT_SRCS))
TBM_LIBFDT_OBJS = $(TBM_LIBFDT_SRCS:.c=.o)

CPPFLAGS += -I $(CURDIR)/$(DRIVER_BOARD)
CPPFLAGS += -I $(CURDIR)/$(PLAT_DIR)
BUILD_SUBDIRS += configs/
BUILD_SUBDIRS += $(LIBVERIF_DIR)
BUILD_SUBDIRS += $(EHASH_DIR)
BUILD_SUBDIRS += $(PLAT_DIR)
BUILD_SUBDIRS += $(TESTSUITE_BOARD)
BUILD_SUBDIRS += $(DRIVER_BOARD)
BUILD_SUBDIRS += $(APPDIR)

HEAD   = $(BUILDDIR)arch-$(ARCH)/head.o
S_OBJS   = ctest-bare.o
S_OBJS  += $(ARCH_ASM_OBJS)
S_OBJS  += unitname.o
S_OBJS  += testcalls.o
S_OBJS  += initcalls.o
S_OBJS  += sys.o
S_OBJS  += plat.o
S_OBJS  += $(ARCH_OBJS)
S_OBJS  += $(TESTSUITE_OBJS)
S_OBJS  += $(TEST_ARCH_OBJS)
S_OBJS  += $(BOARD_OBJS)
S_OBJS  += $(DRIVER_OBJS)
S_OBJS  += $(EHASH_OBJS)
S_OBJS  += $(LIBVERIF_OBJS)
S_OBJS  += $(LIB_OBJS)
S_OBJS  += $(APP_OBJS)

ifeq ($(CONFIG_FDT),y)
S_OBJS  += devtree.o
S_OBJS  += $(TBM_LIBFDT_OBJS)
CPPFLAGS += -I $(CURDIR)/$(LIBFDT_DIR)
BUILD_SUBDIRS += $(LIBFDT_DIR)
endif

OBJS = $(addprefix $(BUILDDIR), $(S_OBJS))
BUILD_SUBDIRS += libminic

TARGET = $(BUILDDIR)ctest-bare
TARGETBIN = $(TARGET).bin
TARGETASM = $(TARGET).asm
TARGETCHDR = $(TARGET).h
TARGET_SYMS_LD = $(TARGET).syms.ld

ifeq ($(OVERRIDE_ALL),)
all: $(TARGETBIN) $(TARGETASM) $(TARGETCHDR) $(TARGET_SYMS_LD) size
endif

include Rules.mk
-include $(APPDIR)/app.mk

.PHONY: libminic-rule
libminic-rule:
	$(MAKE) -C libminic BUILDDIR=../$(BUILDDIR)libminic/ CPUFLAG="$(CPUFLAG)"

BINSECTIONS = .text .data
$(TARGETBIN): $(TARGET)
	@# The binary image needs to get a template of .data
	@# appended to its text section. At boot, the rom code will
	@# copy the .data template into its final link address (_sdata)
	rm -f $@
	touch $@
	for sct in $(BINSECTIONS); do \
		$(OBJCOPY) -O binary -j$${sct} $< $@.tmp && cat $@.tmp >> $@; \
	done
	@$(RM) $@.tmp

$(TARGET_SYMS_LD): $(TARGET)
	./scripts/syms_elf2ldscript.sh $< syms__ >$@

ifeq ($(OVERRIDE_CHDR),)
$(TARGETCHDR): $(TARGETBIN)
	xxd -i $< $@
endif

ifeq ($(OVERRIDE_DISASM),)
$(TARGETASM): $(TARGET)
	$(OBJDUMP) -Srz $< >$@
endif

$(BUILDDIR)libminic/libminic.a: libminic-rule

$(TARGET): $(HEAD) $(OBJS) $(BUILDDIR)libminic/libminic.a $(LDSCRIPT) $(CFG) $(LDDEPS)
	$(call quietprint, $@)
	$(call LINK, $(HEAD) $(OBJS), $@)

size: $(TARGET) $(TARGETBIN)
	$(SIZE)	$(TARGET)
	@ls -al $(TARGETBIN)

libminic-clean:
	$(QUIET)$(MAKE) -C libminic BUILDDIR=../$(BUILDDIR)libminic/ clean

ifeq ($(OVERRIDE_CLEAN),)
.PHONY: clean
clean: libminic-clean
	$(call quietprint, "CLEAN $@")
	$(QUIET)$(RM) $(TARGET) $(TARGETBIN) $(HEAD) $(OBJS) $(OBJS:.o=.d)
endif

distclean:
	$(RM) -r $(BUILD_ROOT)

