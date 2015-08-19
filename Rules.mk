# General Make rules for building TBM
#
# Copyright (C) 2009 - 2014 Edgar E. Iglesias <edgar.iglesias@gmail.com>
#
# BUILDDIR             = Directory where built products end up.
# SRCDIR               = Directory where source code lives.
# BUILD_SUBDIRS        = List of subdires to create on inclusion.

# Provide sane default values.
DEFAULT_RULE ?= all
DTC ?= dtc
CC  ?= cc
LD  ?= cc

ifeq ($V,)
QUIET=@
quietprint=@echo "  $(1)"
.SILENT:
else
QUIET=
quietprint=
endif

# Create BUILD_SUBDIRS
CREATE_BUILDDIRS = $(foreach d, $(BUILD_SUBDIRS), \
	$(shell mkdir -p $(BUILDDIR)$(d)))

# Dereference the variable to create the libs.
$(DEFAULT_RULE): $(CREATE_BUILDDIRS)

# Lining function.
#
#

# Use by $(call $(LINK), objs, output)
LINK = $(QUIET)$(LD) $(1) $(LDFLAGS) $(LDLIBS) -o $(2)

-include $(OBJS:.o=.d)
CPPFLAGS += -MMD

# Compile a DTS into a DTB
$(BUILDDIR)%.dtb: $(SRCDIR)%.dts
	$(call quietprint, $@)
	$(QUIET)$(DTC) $(DTCFLAGS) -I dts -O dtb -o $@ $<

$(BUILDDIR)%.o: $(SRCDIR)%.c
	$(call quietprint, $@)
	$(QUIET)$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@

$(BUILDDIR)%.o: $(SRCDIR)%.S
	$(call quietprint, $@)
	$(QUIET)$(CC) $(ASFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $< -o $@
