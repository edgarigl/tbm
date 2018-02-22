GICV2_SRCS = gic.c handler.c test_bpr.c test_virt.c switch.o

APP_SRCS += $(addprefix testsuite-aarch64/gicv2/,$(GICV2_SRCS))
BUILD_SUBDIRS += testsuite-aarch64/gicv2

