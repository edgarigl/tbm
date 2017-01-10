DG_DDIR=drivers/generic

TESTSUITE_OBJS += $(DG_DDIR)/xilinx-uartlite.o

CPPFLAGS += -I$(DG_DDIR)
BUILD_SUBDIRS += $(DG_DDIR)
