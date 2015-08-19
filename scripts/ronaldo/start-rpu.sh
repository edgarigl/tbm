#!/bin/sh

NAME=rpu
DTS_NAME=dr5

MACHINE_DIR=machine-ronaldo
DTS_DIR=${HOME}/dts

QEMU_ARM=qemu-system-arm
MACH_ARM="-M arm-generic-fdt"
DTB="-hw-dtb ${DTS_DIR}/MULTI_ARCH/ronaldo-${DTS_NAME}.dtb"
SERIAL="-serial mon:stdio -serial null"
MACHINE="-machine-path ${MACHINE_DIR}"

#LOG="-d guest_errors,unimp,exec -D /tmp/${NAME}.log"

set -x
${QEMU_ARM} ${MACH_ARM} ${DTB} ${SERIAL} ${MACHINE} ${LOG} $*
