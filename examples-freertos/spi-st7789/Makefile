# FreeRTOS SPI ST7789 — full driver + logo/ui data (from baremetal)
OUTPUT_DIR := build
PROJECT := freertos-spi-st7789

KERNEL_DIR = ../../FreeRTOS-Kernel
FREERTOS_SHARED_DIR = ../shared
APP_DIR = src
BSP_SHARED = ../../shared

VPATH += $(APP_DIR) $(BSP_SHARED)/drivers $(APP_DIR)/st7789/src $(APP_DIR)/st7789/example
INCLUDE_DIRS += -I$(APP_DIR) -I$(BSP_SHARED)/include
INCLUDE_DIRS += -I$(APP_DIR)/st7789/interface -I$(APP_DIR)/st7789/src -I$(APP_DIR)/st7789/example

SOURCE_FILES += $(APP_DIR)/app_main.c
SOURCE_FILES += $(APP_DIR)/logo.c $(APP_DIR)/ui.c
SOURCE_FILES += $(APP_DIR)/driver_st7789_interface.c
SOURCE_FILES += $(APP_DIR)/st7789/src/driver_st7789.c $(APP_DIR)/st7789/example/driver_st7789_basic.c
SOURCE_FILES += $(BSP_SHARED)/drivers/gpio_driver.c $(BSP_SHARED)/drivers/spi_driver.c

include ../shared/freertos-rv32.mk
