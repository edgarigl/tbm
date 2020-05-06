ifneq "$(AARCH64_CROSS)" ""
export CROSS = $(AARCH64_CROSS)
endif

export CROSS ?= aarch64-linux-gnu-
