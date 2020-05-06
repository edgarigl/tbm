ifneq "$(RV64_CROSS)" ""
export CROSS = $(RV64_CROSS)
endif
export CROSS ?= riscv64-linux-gnu-

ARCH-RISCV = y
ARCH-RISCV64 = y
