#export CROSS = microblazeel-unknown-elf-
ifneq "$(MB_CROSS)" ""
export CROSS = $(MB_CROSS)
endif
export XILINX_TC_VER = 2014.2
export XILINX_TC_VER_DATE = 0606_1
#export CROSS ?= /proj/xbuilds/released/$(XILINX_TC_VER)/$(XILINX_TC_VER)_$(XILINX_TC_VER_DATE)/installs/lin64/SDK/$(XILINX_TC_VER)/gnu/microblaze/linux_toolchain/lin64_le/bin/microblazeel-xilinx-linux-gnu-
export CROSS ?= /proj/xbuilds/2016.3_daily_latest/installs/lin64/SDK/2016.3/gnu/microblaze/lin/bin/microblaze-xilinx-elf-

# Depending on the toolchain you use, you might need to explicitly
# override the default endianness.
CFLAGS  += -mlittle-endian
ASFLAGS += -mlittle-endian
LDFLAGS += -mlittle-endian
#CFLAGS  += -mbig-endian
#ASFLAGS += -mbig-endian
#LDFLAGS += -mbig-endian

# We do have load/store reverse support.
CPPFLAGS += -DHAVE_LDST_REV=1

ARCH-MICROBLAZE = y

