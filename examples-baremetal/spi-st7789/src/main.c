/**
 * ST7789 LCD test via SPI.
 * Initializes display, fills colors, draws rect and string.
 */

#include <stdint.h>
#include <stdio.h>

#include "driver_st7789_basic.h"
#include "gpio_driver.h"
#include "logo.h"
#include "ui.h"
#include "util.h"

/* GPIO: DC = pin 4, RST = pin 5 (used by driver_st7789_interface.c) */
#define PIN_DC 4
#define PIN_RST 5

/* Driver uses 16-bit RGB565: (R<<11)|(G<<5)|B, sent as high byte then low byte
 */
#define RGB565(r5, g6, b5) \
  (((uint32_t)(r5) << 11) | ((uint32_t)(g6) << 5) | (b5))
#define RGB888_TO_RGB565(r, g, b) RGB565((r) >> 3, (g) >> 2, (b) >> 3)

/* Colors in RGB565 (driver expects uint32_t with low 16 bits = RGB565) */
#define COLOR_BLACK 0x0000
#define COLOR_WHITE 0xFFFF
#define COLOR_RED 0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE 0x001F
#define COLOR_YELLOW 0xFFE0
#define COLOR_CYAN 0x07FF
#define COLOR_MAGENTA 0xF81F

st7789_handle_t lcd_handle;

void drawPattern() {
  // Draw a checkerboard pattern in the center
  for (int y = 240; y < 300; y += 10) {
    for (int x = 50; x < 190; x += 10) {
      uint32_t color = ((x / 10 + y / 10) % 2) ? COLOR_WHITE : COLOR_BLACK;
      st7789_fill_rect(&lcd_handle, x, y, x + 10, y + 10, color);
    }
  }
}

void displayDemo() {
  // Draw colored rectangles
  st7789_fill_rect(&lcd_handle, 0, 0, 80, 80, COLOR_RED);
  st7789_fill_rect(&lcd_handle, 80, 0, 160, 80, COLOR_GREEN);
  st7789_fill_rect(&lcd_handle, 160, 0, 240, 80, COLOR_BLUE);

  st7789_fill_rect(&lcd_handle, 0, 80, 80, 160, COLOR_YELLOW);
  st7789_fill_rect(&lcd_handle, 80, 80, 160, 160, COLOR_CYAN);
  st7789_fill_rect(&lcd_handle, 160, 80, 240, 160, COLOR_MAGENTA);

  // Draw some points
  for (int i = 0; i < 50; i++) {
    uint16_t x = 10 + (i * 4);
    uint16_t y = 170 + (i % 10) * 5;
    st7789_draw_point(&lcd_handle, x, y, COLOR_WHITE);
  }

  // Draw a border
  st7789_fill_rect(&lcd_handle, 0, 220, 240, 225, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 0, 315, 240, 320, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 0, 220, 5, 320, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 235, 220, 240, 320, COLOR_WHITE);

  // Display text (if font is available)
  // Note: The write_string function requires font data
  // For now, we'll just draw some patterns
  drawPattern();
}

int main(void) {
  printf("ST7789 test start\n");

  // Initialize the LCD handle
  DRIVER_ST7789_LINK_INIT(&lcd_handle, st7789_handle_t);
  DRIVER_ST7789_LINK_SPI_INIT(&lcd_handle, st7789_interface_spi_init);
  DRIVER_ST7789_LINK_SPI_DEINIT(&lcd_handle, st7789_interface_spi_deinit);
  DRIVER_ST7789_LINK_SPI_WRITE_COMMAND(&lcd_handle,
                                       st7789_interface_spi_write_cmd);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_INIT(
      &lcd_handle, st7789_interface_cmd_data_gpio_init);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_DEINIT(
      &lcd_handle, st7789_interface_cmd_data_gpio_deinit);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_WRITE(
      &lcd_handle, st7789_interface_cmd_data_gpio_write);
  DRIVER_ST7789_LINK_RESET_GPIO_INIT(&lcd_handle,
                                     st7789_interface_reset_gpio_init);
  DRIVER_ST7789_LINK_RESET_GPIO_DEINIT(&lcd_handle,
                                       st7789_interface_reset_gpio_deinit);
  DRIVER_ST7789_LINK_RESET_GPIO_WRITE(&lcd_handle,
                                      st7789_interface_reset_gpio_write);
  DRIVER_ST7789_LINK_DELAY_MS(&lcd_handle, st7789_interface_delay_ms);
  DRIVER_ST7789_LINK_DEBUG_PRINT(&lcd_handle, st7789_interface_debug_print);

  // Initialize the LCD
  if (st7789_init(&lcd_handle) != 0) {
    printf("LCD initialization failed!\n");
    while (1);  // Halt if initialization fails
  }

  // Set display size (240x320 is common for ST7789)
  st7789_set_column(&lcd_handle, 240);
  st7789_set_row(&lcd_handle, 320);

  // Wake up the display
  st7789_sleep_out(&lcd_handle);
  delay_ms(200);

  // Configure display settings
  st7789_idle_mode_off(&lcd_handle);
  st7789_normal_display_mode_on(&lcd_handle);
  st7789_display_inversion_on(&lcd_handle);
  st7789_set_gamma(&lcd_handle, ST7789_GAMMA_CURVE_1);

  // Set memory access control
  st7789_set_memory_data_access_control(
      &lcd_handle,
      ST7789_ORDER_PAGE_TOP_TO_BOTTOM | ST7789_ORDER_COLUMN_LEFT_TO_RIGHT |
          ST7789_ORDER_PAGE_COLUMN_NORMAL | ST7789_ORDER_LINE_TOP_TO_BOTTOM |
          ST7789_ORDER_COLOR_RGB | ST7789_ORDER_REFRESH_LEFT_TO_RIGHT);

  // Set pixel format to 16-bit
  st7789_set_interface_pixel_format(
      &lcd_handle, ST7789_RGB_INTERFACE_COLOR_FORMAT_262K,
      ST7789_CONTROL_INTERFACE_COLOR_FORMAT_16_BIT);

  // Turn on display
  st7789_display_on(&lcd_handle);
  delay_ms(100);

  // Clear screen with black background
  st7789_clear(&lcd_handle);
  delay_ms(100);

  // Display some content
  displayDemo();
  delay_ms(3000);

  st7789_clear(&lcd_handle);

  st7789_draw_picture_16bits(&lcd_handle, 48, 0, 48 + LOGO_WIDTH - 1,
                             LOGO_HEIGHT - 1, (uint16_t *)logo_map);

  delay_ms(3000);

  st7789_draw_picture_16bits(&lcd_handle, 0, 0, UI_WIDTH - 1, UI_HEIGHT - 1,
                             (uint16_t *)ui_map);

  printf("ST7789 test done\n");

  gpio_set_direction(0, GPIO_OUTPUT);
  gpio_set_pin(0);

  while (1) {
    for (volatile int i = 0; i < 100000; i++) {
    }
    gpio_toggle_pin(0);
  }
}
