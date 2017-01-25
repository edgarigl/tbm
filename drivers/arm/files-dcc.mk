DDIR=drivers/arm

DRIVER_SRCS += $(DDIR)/dcc.c

BUILD_SUBDIRS += $(DDIR)
CPPFLAGS += -I $(CURDIR)/$(DDIR)
