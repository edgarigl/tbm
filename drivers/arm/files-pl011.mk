DRIVER_SRCS += drivers/arm/pl011.c

BUILD_SUBDIRS += drivers/arm
CPPFLAGS += -I $(CURDIR)/drivers/arm
