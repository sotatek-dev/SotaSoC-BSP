# FreeRTOS HelloWorld — paths relative to build/gcc/
OUTPUT_DIR := build
PROJECT := freertos-blink

# Project sets kernel and shared paths (required by freertos-rv32.mk)
KERNEL_DIR = ../../FreeRTOS-Kernel
FREERTOS_SHARED_DIR = ../shared

# App sources and includes (path to this blink from build/gcc)
APP_DIR = .
BSP_SHARED = ../../shared
VPATH += $(APP_DIR) $(BSP_SHARED)/drivers
INCLUDE_DIRS += -I$(APP_DIR) -I$(BSP_SHARED)/include
SOURCE_FILES += $(APP_DIR)/app_main.c $(BSP_SHARED)/drivers/gpio_driver.c
include ../shared/freertos-rv32.mk
