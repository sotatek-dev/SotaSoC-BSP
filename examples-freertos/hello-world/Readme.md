# Emulating generic RISC-V 32bit machine on QEMU

## Requirements

1. GNU RISC-V toolchains Tested on:
  * Pre-built Sifive GNU Embedded Toolchain — v3.0.4 - https://www.sifive.com/software
  * Self built from https://github.com/riscv-collab/riscv-gnu-toolchain/tree/a33dac0251d17a7b74d99bd8fd401bfce87d2aed (tag: 2025.01.20)

1. Linux OS. Tested on:
  * Ubuntu 24.04 LTS


## How to build

Add the toolchain `bin` directory to your `PATH`. Default compiler prefix is `riscv32-unknown-elf-`; to use another (e.g. `riscv-none-elf-`):

```bash
export PATH="/path/to/toolchain/bin:$PATH"
# Optional: different prefix
export CROSS_COMPILE="riscv-none-elf-"

make -C build/gcc/
```

