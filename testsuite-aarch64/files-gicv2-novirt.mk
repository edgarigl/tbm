TESTSUITE_OBJS += testsuite-aarch64/check_gicv2.o

APP_SRCS += $(addprefix testsuite-aarch64/gicv2/, gic.c handler.c test_bpr.c switch.o)
BUILD_SUBDIRS += testsuite-aarch64/gicv2
