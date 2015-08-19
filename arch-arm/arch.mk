ifneq "$(ARM_CROSS)" ""
export CROSS = $(ARM_CROSS)
endif

export CROSS ?= arm-none-eabi-
