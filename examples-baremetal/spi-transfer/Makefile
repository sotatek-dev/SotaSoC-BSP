# SPI Example Makefile

# Project name
PROJECT := spi-transfer

# Bare-metal: no libc
USE_STDLIB := 0

# Source files
SRCS = \
	main.c \
	spi_driver.c \
	gpio_driver.c

# Drivers used (adds drivers directory to VPATH)
DRIVERS = spi_driver.c gpio_driver.c

# Include common Makefile
include ../shared/common.mk
