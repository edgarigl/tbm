#!/bin/sh

NAME=pmu
DTS_NAME=${NAME}

MACHINE_DIR=machine-ronaldo
DTS_DIR=${HOME}/dts

QEMU_MBEL=qemu-system-microblazeel
MACH_MB="-M microblaze-fdt"
DTB="-hw-dtb ${DTS_DIR}/MULTI_ARCH/ronaldo-${DTS_NAME}.dtb"
SERIAL="-serial mon:stdio -serial null"
MACHINE="-machine-path ${MACHINE_DIR}"

LOG="-d guest_errors,unimp -D /tmp/${NAME}.log"

set -x
${QEMU_MBEL} ${MACH_MB} ${DTB} ${SERIAL} ${MACHINE} $*
