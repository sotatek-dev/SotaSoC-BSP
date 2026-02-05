/*
 * FreeRTOS SPI ST7789 — full driver + logo/ui (same as baremetal demo).
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "driver_st7789_basic.h"
#include "logo.h"
#include "ui.h"

#define DISPLAY_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 4 )

#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F
#define COLOR_YELLOW 0xFFE0
#define COLOR_CYAN   0x07FF
#define COLOR_MAGENTA 0xF81F

static st7789_handle_t lcd_handle;
static void displayTask(void *param);

static void drawPattern(void) {
  for (int y = 240; y < 300; y += 10) {
    for (int x = 50; x < 190; x += 10) {
      uint32_t color = ((x / 10 + y / 10) % 2) ? COLOR_WHITE : COLOR_BLACK;
      st7789_fill_rect(&lcd_handle, x, y, x + 10, y + 10, color);
    }
  }
}

static void displayDemo(void) {
  st7789_fill_rect(&lcd_handle, 0, 0, 80, 80, COLOR_RED);
  st7789_fill_rect(&lcd_handle, 80, 0, 160, 80, COLOR_GREEN);
  st7789_fill_rect(&lcd_handle, 160, 0, 240, 80, COLOR_BLUE);
  st7789_fill_rect(&lcd_handle, 0, 80, 80, 160, COLOR_YELLOW);
  st7789_fill_rect(&lcd_handle, 80, 80, 160, 160, COLOR_CYAN);
  st7789_fill_rect(&lcd_handle, 160, 80, 240, 160, COLOR_MAGENTA);

  for (int i = 0; i < 50; i++) {
    uint16_t x = 10 + (i * 4);
    uint16_t y = 170 + (i % 10) * 5;
    st7789_draw_point(&lcd_handle, x, y, COLOR_WHITE);
  }

  st7789_fill_rect(&lcd_handle, 0, 220, 240, 225, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 0, 315, 240, 320, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 0, 220, 5, 320, COLOR_WHITE);
  st7789_fill_rect(&lcd_handle, 235, 220, 240, 320, COLOR_WHITE);
  drawPattern();
}

void vApplicationStart(void) {
  xTaskCreate(displayTask, "Display", DISPLAY_TASK_STACK_SIZE, NULL, 8, NULL);

  printf("FreeRTOS SPI ST7789 starting...\n");
  vTaskStartScheduler();

  for (;;)
    printf("Scheduler ended unexpectedly.\n");
}

static void displayTask(void *param) {
  (void)param;

  DRIVER_ST7789_LINK_INIT(&lcd_handle, st7789_handle_t);
  DRIVER_ST7789_LINK_SPI_INIT(&lcd_handle, st7789_interface_spi_init);
  DRIVER_ST7789_LINK_SPI_DEINIT(&lcd_handle, st7789_interface_spi_deinit);
  DRIVER_ST7789_LINK_SPI_WRITE_COMMAND(&lcd_handle, st7789_interface_spi_write_cmd);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_INIT(&lcd_handle, st7789_interface_cmd_data_gpio_init);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_DEINIT(&lcd_handle, st7789_interface_cmd_data_gpio_deinit);
  DRIVER_ST7789_LINK_COMMAND_DATA_GPIO_WRITE(&lcd_handle, st7789_interface_cmd_data_gpio_write);
  DRIVER_ST7789_LINK_RESET_GPIO_INIT(&lcd_handle, st7789_interface_reset_gpio_init);
  DRIVER_ST7789_LINK_RESET_GPIO_DEINIT(&lcd_handle, st7789_interface_reset_gpio_deinit);
  DRIVER_ST7789_LINK_RESET_GPIO_WRITE(&lcd_handle, st7789_interface_reset_gpio_write);
  DRIVER_ST7789_LINK_DELAY_MS(&lcd_handle, st7789_interface_delay_ms);
  DRIVER_ST7789_LINK_DEBUG_PRINT(&lcd_handle, st7789_interface_debug_print);

  if (st7789_init(&lcd_handle) != 0) {
    printf("LCD init failed!\n");
    vTaskDelete(NULL);
    return;
  }

  st7789_set_column(&lcd_handle, 240);
  st7789_set_row(&lcd_handle, 320);
  st7789_sleep_out(&lcd_handle);
  vTaskDelay(pdMS_TO_TICKS(200));

  st7789_idle_mode_off(&lcd_handle);
  st7789_normal_display_mode_on(&lcd_handle);
  st7789_display_inversion_on(&lcd_handle);
  st7789_set_gamma(&lcd_handle, ST7789_GAMMA_CURVE_1);
  st7789_set_memory_data_access_control(
      &lcd_handle,
      ST7789_ORDER_PAGE_TOP_TO_BOTTOM | ST7789_ORDER_COLUMN_LEFT_TO_RIGHT |
          ST7789_ORDER_PAGE_COLUMN_NORMAL | ST7789_ORDER_LINE_TOP_TO_BOTTOM |
          ST7789_ORDER_COLOR_RGB | ST7789_ORDER_REFRESH_LEFT_TO_RIGHT);
  st7789_set_interface_pixel_format(
      &lcd_handle, ST7789_RGB_INTERFACE_COLOR_FORMAT_262K,
      ST7789_CONTROL_INTERFACE_COLOR_FORMAT_16_BIT);
  st7789_display_on(&lcd_handle);
  vTaskDelay(pdMS_TO_TICKS(100));

  st7789_clear(&lcd_handle);
  vTaskDelay(pdMS_TO_TICKS(100));

  displayDemo();
  vTaskDelay(pdMS_TO_TICKS(3000));

  st7789_clear(&lcd_handle);

  st7789_draw_picture_16bits(&lcd_handle, 48, 0, 48 + LOGO_WIDTH - 1, LOGO_HEIGHT - 1, (uint16_t *)logo_map);
  vTaskDelay(pdMS_TO_TICKS(3000));

  st7789_draw_picture_16bits(&lcd_handle, 0, 0, UI_WIDTH - 1, UI_HEIGHT - 1, (uint16_t *)ui_map);
  printf("ST7789 demo done\n");

  for (;;)
    vTaskDelay(pdMS_TO_TICKS(1000));
}
