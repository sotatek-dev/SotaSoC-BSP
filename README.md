# SotaSoC-BSP

Board Support Package (BSP) for **[SotaSoC](https://github.com/sotatek-dev/SotaSoC)** — a compact RISC-V RV32I SoC for Tiny Tapeout and Artix 7 FPGA. This BSP provides drivers, startup code, linker scripts, and bare-metal / FreeRTOS examples that target SotaSoC’s memory map and peripherals.

## Features

- **Target:** SotaSoC (RV32I + C + Zicsr). QSPI Flash at `0x00000000`, PSRAM at `0x01000000`.
- **Toolchain:** `rv32ic_zicsr`, ABI `ilp32`, with `zicsr` support.
- **Memory (linker):** ROM `0x00000000` (16 MB), RAM `0x01000000` (16 MB).
- **Peripherals** (see `shared/include/hw_registers.h`): UART, GPIO, Timer, PWM, I2C, SPI — aligned with SotaSoC’s memory map.
- **Example sets:** Bare-metal and FreeRTOS.

### Pinout

| Pin | Input | Output | Bidirectional |
|-----|-------|--------|----------------|
| 0 | SPI_MISO | UART0_TX | FLASH_CS_N |
| 1 | GPIO_IN[0] | ERROR_FLAG | BUS_IO[0] |
| 2 | GPIO_IN[1] | GPIO_OUT[0] / I2C_SCL | BUS_IO[1] |
| 3 | GPIO_IN[2] | GPIO_OUT[1] / SPI_CS_N | BUS_SPI_SCLK |
| 4 | GPIO_IN[3] | GPIO_OUT[2] / SPI_SCLK | BUS_IO[2] |
| 5 | GPIO_IN[4] | GPIO_OUT[3] / SPI_MOSI | BUS_IO[3] |
| 6 | GPIO_IN[5] | GPIO_OUT[4] / PWM[0] | RAM_CS_N |
| 7 | UART0_RX | GPIO_OUT[5] | GPIO_IO[0] / I2C_SDA |

**Note:** In code, **GPIO_IO[0]** is `GPIO[0]`; **GPIO_IN[*x*]** and **GPIO_OUT[*x*]** correspond to `GPIO[x+1]`.

## Requirements

### Toolchain

To use libc, the toolchain must include **newlib**. The one from `apt` usually does not, so you need a toolchain that supports newlib:

- **Option 1:** [Build it yourself](toolchain/build-toolchain-newlib.md).
- **Option 2:** Use the [pre-built toolchain](toolchain/riscv-newlib-15.2.0.zip); extract it to `/opt/riscv-newlib`.

Add the toolchain `bin` directory to your `PATH` so the build can find the compiler. For example, if the toolchain is in `/opt/riscv-newlib`, run:

```bash
export PATH="/opt/riscv-newlib/bin:$PATH"
```

To make this permanent, add that line to your shell config (e.g. `~/.bashrc`), then run `source ~/.bashrc` or open a new terminal.

### Host OS

Tested on **Ubuntu 24.04 LTS**. Other Linux environments may work with the same toolchain.

## Quick start

1. Install or build a RISC-V toolchain with newlib (see [Requirements](#requirements)) and add its `bin` to `PATH`.
2. Go into an example's folder and build:

```bash
cd examples-baremetal/gpio
make clean && make
```

or for FreeRTOS (e.g. hello-world, blink, spi-st7789):

```bash
cd examples-freertos/hello-world
make clean && make
```

3. Outputs (`.elf`, `.bin`) are in that example’s `build/` directory.

## Repository structure

| Path | Description |
|------|-------------|
| `shared/` | Shared BSP code: trap handler, drivers (GPIO, I2C, PWM, SPI), `hw_registers.h`, util. |
| `examples-baremetal/` | Bare-metal examples (blink, gpio, printf, i2c, spi, pwm, …). Each subdirectory has a Makefile that includes `shared/common.mk`. |
| `examples-baremetal/shared/` | Startup, linker script, syscalls, `common.mk`. |
| `examples-freertos/` | FreeRTOS examples (hello-world, blink, spi-st7789). Use `shared/freertos-rv32.mk` and the kernel in `FreeRTOS-Kernel/`. |
| `examples-freertos/shared/` | FreeRTOS shared: startup, linker script, `freertos-rv32.mk`. |

## Examples

| Example | Type | Description |
|---------|------|-------------|
| [blink](examples-baremetal/blink) | Bare metal | LED blink (polling) example. |
| [blink-int](examples-baremetal/blink-int) | Bare metal | LED blink with interrupts. |
| [gpio](examples-baremetal/gpio) | Bare metal | GPIO driver demo. |
| [gpio-int](examples-baremetal/gpio-int) | Bare metal | Button on pin 1 triggers a GPIO interrupt to toggle LED on pin 0; includes software debounce. |
| [printf](examples-baremetal/printf) | Bare metal | printf over UART. |
| [i2c-lcd1602](examples-baremetal/i2c-lcd1602) | Bare metal | Drives a 16×2 character LCD with PCF8574 I2C backpack. |
| [i2c-transfer](examples-baremetal/i2c-transfer) | Bare metal | I2C driver test: read/write a byte to a 7-bit slave address; demonstrates repeated START. |
| [spi-transfer](examples-baremetal/spi-transfer) | Bare metal | SPI driver test. |
| [spi-flash](examples-baremetal/spi-flash) | Bare metal | Reads data from SPI NOR Flash. |
| [spi-st7789](examples-baremetal/spi-st7789) | Bare metal | Drive ST7789 color TFT over SPI at 16MHz. |
| [pwm](examples-baremetal/pwm) | Bare metal | PWM demo (breathing LED effect). |
| [hello-world](examples-freertos/hello-world) | FreeRTOS | FreeRTOS demo (tasks, queues). |
| [blink](examples-freertos/blink) | FreeRTOS | LED blink with FreeRTOS tasks. |
| [spi-st7789](examples-freertos/spi-st7789) | FreeRTOS | Drive ST7789 color TFT over SPI with FreeRTOS. |

## Build options

*(Bare-metal only.)* Use **`USE_STDLIB=1`** (default) to link with newlib, or **`USE_STDLIB=0`** for nostdlib (you will cannot use stdlib functions like printf, ...).

## Flashing and running on hardware

Program the `.bin` file to QSPI Flash at address `0x0000_0000`. Then reset the FPGA or development board to start the program.

- **Running on FPGA:** See [SotaSoC §5 Building and running on FPGA](https://github.com/sotatek-dev/SotaSoC?tab=readme-ov-file#5-building-and-running-on-fpga).
- **Running on Tiny Tapeout development board:** See [How to test](https://github.com/sotatek-dev/ttihp-SotaSoC/blob/main/docs/info.md#how-to-test).

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for the full text. Contributions are welcome; please open an issue or pull request.
