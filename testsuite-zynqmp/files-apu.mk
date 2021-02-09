TDIR=testsuite-$(BOARD)

TESTSUITE_OBJS += $(TDIR)/check_ronaldo.o
TESTSUITE_OBJS += $(TDIR)/check_bram_exec.o
TESTSUITE_OBJS += $(TDIR)/check_zdma.o
TESTSUITE_OBJS += $(TDIR)/check_qspi.o

BUILD_SUBDIRS += $(TDIR)
