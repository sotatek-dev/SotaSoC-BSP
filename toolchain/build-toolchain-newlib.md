# Build GCC RISC-V with Newlib

The Ubuntu toolchain (`apt install gcc-riscv64-unknown-elf`) is built **without newlib**. To use `stdio.h`, `printf`, and libc with **ilp32** (rv32ic), you need to build the toolchain from source.

## 1. Install dependencies (Ubuntu/Debian)

```bash
sudo apt install autoconf automake autotools-dev curl python3 libmpc-dev \
  libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf \
  libtool patchutils bc zlib1g-dev libexpat-dev
```

## 2. Clone riscv-gnu-toolchain

```bash
git clone https://github.com/riscv-collab/riscv-gnu-toolchain
cd riscv-gnu-toolchain
# Newlib is a submodule; clone it fully
git submodule update --init --recursive
```

## 3. Build with Newlib, target rv32ic/ilp32

Install to a dedicated directory (e.g. `/opt/riscv-newlib`):

```bash
mkdir build && cd build
../configure --prefix=/opt/riscv-newlib \
  --with-arch=rv32ic \
  --with-abi=ilp32
make -j$(nproc)
```

After the build, add to PATH:

```bash
export PATH=/opt/riscv-newlib/bin:$PATH
```

Verify:

```bash
riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc -print-file-name=include
# Should show the path to the include directory containing stdio.h
```
