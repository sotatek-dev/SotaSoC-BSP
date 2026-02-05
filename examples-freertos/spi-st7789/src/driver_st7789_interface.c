/**
 * ST7789 interface for FreeRTOS: SPI + GPIO.
 * Uses vTaskDelay instead of delay_ms (no util.h).
 */
#include "driver_st7789_interface.h"
#include "spi_driver.h"
#include "gpio_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#define CS_N_PIN 2
#define DC_PIN 5
#define RST_PIN 6

uint8_t st7789_interface_spi_init(void) {
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);
  spi_init(SPI_DIV_16MHZ);
  return 0;
}

uint8_t st7789_interface_spi_deinit(void) {
  return 0;
}

uint8_t st7789_interface_spi_write_cmd(uint8_t *buf, uint32_t len) {
  gpio_clear_pin(CS_N_PIN);

  unsigned int pos = 0u;
  unsigned int align_off = (unsigned int)((uintptr_t)buf & 3u);
  unsigned int lead = (align_off == 0u) ? 0u : (unsigned int)(4u - align_off);
  if (lead > len) lead = len;
  for (unsigned int i = 0u; i < lead; i++) {
    spi_transfer_byte(buf[pos++]);
  }
  unsigned int rest = len - pos;
  unsigned int fast_len = rest & ~3u;
  if (fast_len != 0u) {
    spi_transmit_fast(buf + pos, (int)fast_len);
    pos += fast_len;
  }
  rest = len - pos;
  for (unsigned int i = 0u; i < rest; i++) {
    spi_transfer_byte(buf[pos++]);
  }

  gpio_set_pin(CS_N_PIN);
  return 0;
}

void st7789_interface_delay_ms(uint32_t ms) {
  vTaskDelay(pdMS_TO_TICKS(ms));
}

void st7789_interface_debug_print(const char *const fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

uint8_t st7789_interface_cmd_data_gpio_init(void) {
  gpio_write_pin(DC_PIN, 0);
  return 0;
}

uint8_t st7789_interface_cmd_data_gpio_deinit(void) {
  return 0;
}

uint8_t st7789_interface_cmd_data_gpio_write(uint8_t value) {
  gpio_write_pin(DC_PIN, value ? 1 : 0);
  return 0;
}

uint8_t st7789_interface_reset_gpio_init(void) {
  gpio_set_direction(RST_PIN, GPIO_OUTPUT);
  gpio_set_pin(RST_PIN);
  return 0;
}

uint8_t st7789_interface_reset_gpio_deinit(void) {
  return 0;
}

uint8_t st7789_interface_reset_gpio_write(uint8_t value) {
  gpio_write_pin(RST_PIN, value ? 1 : 0);
  return 0;
}
