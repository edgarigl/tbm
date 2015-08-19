#!/bin/bash
#
# Break out of ssiv/settings.sh
#

QEMU=$1
OUTDIR=$2

DTC=dtc
set -e

for V in MULTI_ARCH SINGLE_ARCH SINGLE_ARCH_A9 ZYNQ LQSPI_XIP LINUX_HAX COSIM; do
for R in LATEST "3-1"; do
	echo
	echo ${R}/${V}

    GCC_DFLAGS="-D${V}"
    if [ "${V}" == "LQSPI_XIP" ]; then
        GCC_DFLAGS="${GCC_DFLAGS} -DMULTI_ARCH";
    fi;
    if [ "${V}" == "LINUX_HAX" ]; then
        GCC_DFLAGS="${GCC_DFLAGS} -DSINGLE_ARCH";
    fi;
    if [ "${V}" == "SINGLE_ARCH_A9" ]; then
        GCC_DFLAGS="${GCC_DFLAGS} -DSINGLE_ARCH";
    fi;

        rm -rf pcbios_tmp
        mkdir ./pcbios_tmp
        cp -r $QEMU/pc-bios/*.dts* ./pcbios_tmp

        pushd pcbios_tmp > /dev/null

        for I in $(find . -name "*.dts*"); do
        ALTERNATE="$(basename ${I} .dtsh)-${R}.dtsh"
        if [ -e ${ALTERNATE} ]; then
            mv ${ALTERNATE} ${I};
        fi
        done;

    for I in $(find . -name "*.dts*"); do
            if [ "$(echo $I | grep -c '\.dtsh')" == "0" ]; then
                    mv ${I} ${I}.c
                    sed "s/#/_.$$._/g" -i ${I}.c
                    sed "s/_.$$.__.$$._/#/g" -i ${I}.c
                    gcc -E -P -C ${GCC_DFLAGS} -Ulinux -o ${I}.$$.tmp ${I}.c
                    mv ${I}.$$.tmp ${I}
                    sed "s/_.$$._/#/g" -i ${I}
                fi
        done;

        for I in $(find . -name "*.dts" | sed 's/dts$//g'); do
                echo ${DTC} -I dts < ${I}dts -o $(basename ${I})dtb -O dtb
                ${DTC} -I dts < ${I}dts -o ${I}dtb -O dtb
                NAME=`basename ${I}`
                ${DTC} -I dts < ${I}dts -o xilinx-${NAME}dts -O dts
        done;

        popd > /dev/null

	mkdir -p $OUTDIR//${R}/${V}
        mv pcbios_tmp/* $OUTDIR/${R}/${V}
        rm -rf pcbios_tmp
done;
done;
