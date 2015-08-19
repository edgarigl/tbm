DDIR=drivers/arm

DRIVER_SRCS += $(DDIR)/smmu.c

BUILD_SUBDIRS += $(DDIR)
CPPFLAGS += -I $(CURDIR)/$(DDIR)
