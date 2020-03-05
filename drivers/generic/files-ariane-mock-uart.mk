DG_DDIR=drivers/generic

TESTSUITE_OBJS += $(DG_DDIR)/ariane-mock-uart.o

CPPFLAGS += -I$(DG_DDIR)
BUILD_SUBDIRS += $(DG_DDIR)
