# SPI ST7789 LCD Example

This example demonstrates controlling an ST7789 LCD via SPI at 16 MHz, achieving approximately ~10 FPS. It uses the [libdriver/st7789](https://github.com/libdriver/st7789) driver and shows basic graphics on the display.

## 1. Wiring: LCD to Development Board

Connect the LCD to the development board as follows:

| LCD Pin | Development Board Pin |
|---------|------------------------|
| VCC     | VCC                    |
| GND     | GND                    |
| CS      | uo[3]                  |
| SCK     | uo[4]                  |
| SDI (MOSI) | uo[5]               |
| DC      | uo[6]                  |
| RST     | uo[7]                  |
| LED     | VCC                    |

## 2. Loading the Firmware

Program the built firmware to flash at address `0x00000000`:

- **Firmware:** [build/spi-st7789.bin](build/spi-st7789.bin)


## 3. Expected Result

After resetting the development board, the LCD should show the demo screen as in the image below.

![SPI ST7789 demo result](docs/images/st7789-demo.jpeg)

*The photo above was taken from a test on an Artix 7 FPGA; the tapeout chip is not yet available.*
