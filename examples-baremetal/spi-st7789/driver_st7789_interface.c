/**
 *Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_st7789_interface_template.c
 * @brief     driver st7789 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-04-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/04/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_st7789_interface.h"
#include "spi_driver.h"
#include "gpio_driver.h"
#include "util.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#define CS_N_PIN 2
#define DC_PIN 5
#define RST_PIN 6

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t st7789_interface_spi_init(void)
{
  gpio_set_direction(CS_N_PIN, GPIO_OUTPUT);
  gpio_set_pin(CS_N_PIN);  // CS_N high (deasserted)
  spi_init(SPI_DIV_16MHZ);
  return 0;
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t st7789_interface_spi_deinit(void)
{
    return 0;
}

/**
 * @brief     interface spi bus write
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t st7789_interface_spi_write_cmd(uint8_t *buf, uint32_t len)
{
  gpio_clear_pin(CS_N_PIN);

  /* 16MHz: spi_transmit_fast(tx_buf, len) expects len in bytes, len % 4 == 0; buf 4-byte aligned */
  unsigned int pos = 0u;

  /* Lead bytes to reach 4-byte alignment */
  unsigned int align_off = (unsigned int)((uintptr_t)buf & 3u);
  unsigned int lead = (align_off == 0u) ? 0u : (unsigned int)(4u - align_off);
  if (lead > len) lead = len;
  for (unsigned int i = 0u; i < lead; i++) {
    spi_transfer_byte(buf[pos++]);
  }
  unsigned int rest = len - pos;
  unsigned int fast_len = rest & ~3u;  /* multiple of 4 for fast path */
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

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void st7789_interface_delay_ms(uint32_t ms)
{
  delay_ms((int)ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void st7789_interface_debug_print(const char *const fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

/**
 * @brief  interface command && data gpio init
 * @return status code
 *         - 0 success
 *         - 1 gpio init failed
 * @note   none
 */
uint8_t st7789_interface_cmd_data_gpio_init(void)
{
    gpio_write_pin(DC_PIN, 0);
    return 0;
}

/**
 * @brief  interface command && data gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio deinit failed
 * @note   none
 */
uint8_t st7789_interface_cmd_data_gpio_deinit(void)
{
    return 0;
}

/**
 * @brief     interface command && data gpio write
 * @param[in] value written value
 * @return    status code
 *            - 0 success
 *            - 1 gpio write failed
 * @note      none
 */
uint8_t st7789_interface_cmd_data_gpio_write(uint8_t value)
{
  gpio_write_pin(DC_PIN, value ? 1 : 0);
  return 0;
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 gpio init failed
 * @note   none
 */
uint8_t st7789_interface_reset_gpio_init(void)
{
    gpio_set_direction(RST_PIN, GPIO_OUTPUT);
    gpio_set_pin(RST_PIN);  // RST high (deasserted)
    return 0;
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio deinit failed
 * @note   none
 */
uint8_t st7789_interface_reset_gpio_deinit(void)
{
    return 0;
}

/**
 * @brief     interface reset gpio write
 * @param[in] value written value
 * @return    status code
 *            - 0 success
 *            - 1 gpio write failed
 * @note      none
 */
uint8_t st7789_interface_reset_gpio_write(uint8_t value)
{
  gpio_write_pin(RST_PIN, value ? 1 : 0);
  return 0;
}
