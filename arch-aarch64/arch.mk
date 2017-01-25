ifneq "$(AARCH64_CROSS)" ""
export CROSS = $(AARCH64_CROSS)
endif

export CROSS ?= /tools/gensys/aarch64/arm64_none/bin/aarch64-none-elf-
