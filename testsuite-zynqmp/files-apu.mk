TDIR=testsuite-$(BOARD)

TESTSUITE_OBJS += $(TDIR)/check_ronaldo.o
TESTSUITE_OBJS += $(TDIR)/check_zdma.o

BUILD_SUBDIRS += $(TDIR)
