DG_DDIR=drivers/generic

TESTSUITE_OBJS += $(DG_DDIR)/xen-emergency-console.o

CPPFLAGS += -I$(DG_DDIR)
BUILD_SUBDIRS += $(DG_DDIR)
