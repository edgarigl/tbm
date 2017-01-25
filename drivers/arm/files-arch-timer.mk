DRIVER_SRCS += drivers/arm/arch-timer.c

BUILD_SUBDIRS += drivers/arm
CPPFLAGS += -I $(CURDIR)/drivers/arm
