ifneq "$(RV32_CROSS)" ""
export CROSS = $(RV32_CROSS)
endif
export CROSS ?= riscv32-unknown-elf-

ARCH-RISCV = y
ARCH-RISCV32 = y
