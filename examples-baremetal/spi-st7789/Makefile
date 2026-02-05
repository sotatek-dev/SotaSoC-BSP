# SPI ST7789 Example Makefile

# Project name
PROJECT := spi-st7789

# Include paths: shared BSP + ST7789 (interface, src, example)
INCLUDES := ../../shared/include src src/st7789/interface src/st7789/src src/st7789/example

# Source files: app + ST7789 driver + shared drivers
SRCS = \
	main.c \
	ui.c \
	logo.c \
	spi_driver.c \
	gpio_driver.c \
	driver_st7789_interface.c \
	driver_st7789.c \
	driver_st7789_basic.c \
	util.c

# Drivers used (adds drivers directory to VPATH)
DRIVERS = spi_driver.c gpio_driver.c

# VPATH for ST7789 .c in subdirs; BSP shared (util.c)
VPATH += src src/st7789/src src/st7789/example
VPATH += ../../shared

# Include common Makefile
include ../shared/common.mk
