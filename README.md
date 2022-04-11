# Test-BareMetal (TBM)

This is a small bare-metal SW test/exploration environment.

## Dependencies

```
apt-get install gcc-arm-none-eabi gcc-aarch64-linux-gnu \
	gcc-riscv64-linux-gnu
```

## Build targets

Examples how-to build some ARM and MicroBlaze variants:
$ make CFG=configs/vexpress-a32.cfg
$ make CFG=configs/vexpress-a64.cfg
$ make CFG=configs/petalogix-s3adsp1800.cfg

To build all:
```make CFG=configs/test```

ZynqMP APU setup:
```make CFG=configs/zynqmp-apu.cfg```

If you need to override the default toolchains:
```
make CFG=configs/vexpress-a64.cfg CROSS=aarch64-myown-elf-
```

Or you can create a .config file, for example:
```
$ cat .config
MB_CROSS=microblazeel-xilinx-linux-gnu-
ARM_CROSS=arm-xilinx-linux-gnueabi-
AARCH64_CROSS=aarch64-linux-gnu-
```

The final binaries can be found in the build/* dir.

## Versal

### APU
```
qemu-system-aarch64 -M xlnx-versal-virt -serial stdio -display none -kernel build/versal/apu/ctest-bare
```

### RPU

We need to load a small application for the APU that releases the RPU from reset.
```
qemu-system-aarch64 -M xlnx-versal-virt -serial stdio -display none -kernel build/versal/apu_release_rpu/ctest-bare -device loader,file=./build/versal/rpu/ctest-bare,cpu-num=2
```

## ZynqMP

### APU
```
qemu-system-aarch64 -M xlnx-zcu102,secure=on,virtualization=on -serial stdio \
	 -kernel build/zynqmp/apu/ctest-bare
```

### RPU
```
qemu-system-aarch64 -M xlnx-zcu102 -smp 6 -serial stdio \
	-device loader,file=./build/zynqmp/rpu/ctest-bare,cpu-num=4 \
	-global xlnx,zynqmp.boot-cpu="rpu-cpu[0]"
```

To run the vexpress-a64 tests, do the following:
```
qemu-system-aarch64 -M vexpress-a15,secure=on -cpu cortex-a53 -m 1024 \
	-display none -serial stdio -kernel build/vexpress/a64/ctest-bare
```

To run the vexpress-a32 tests, do the following:
```
qemu-system-arm -M vexpress-a15,secure=on -serial stdio -display none \
	-kernel build/vexpress/a32/ctest-bare
```

## RISCv64

### Build
```
make CFG=configs/ariane.cfg
```

### RISCv64 on QEMU
```
qemu-system-riscv64 -M virt -m 256M -serial stdio -display none \
	-device loader,file=./build/ariane_soc/ctest-bare,cpu-num=0
```

### RISCv64 on Ariane test-harness
```
./work-ver/Variane_testharness ~/src/c/tbm/tbm/build/ariane_soc/ctest-bare
```

## RISCv32

### Build
```
make CFG=configs/picorv32.cfg
```

### RISCv32 on the Picorv32 test bench
```
./testbench_verilator +firmware=tbm/build/picorv32/ctest-bare.hex 
```

## MicroBlaze
To run the petalogix-s3adsp1800 tests, run the following:
```
qemu-system-microblazeel -M petalogix-s3adsp1800 -serial stdio \
	-kernel build/petalogix-s3adsp1800/main/ctest-bare
```

## Directory structure

* configs/ Build configuration files.
* build/ Build products/artifacts.
* arch-*/ Contains arch specific boot code, interrupt setups and other stuff.
* testsuite-*/ Contains most of the test driving logic.
* drivers/*/ Contains drivers to be used by tests or other code.
* drivers-*/ Legacy naming for old drivers
* libminic/ A BSD licenced small C library implementation
* libfdt/ Flat Device Tree lib to parse device trees on boot
