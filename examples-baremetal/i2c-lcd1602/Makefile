# I2C LCD1602 Example Makefile

# Project name
PROJECT := i2c-lcd1602

# Include path: current dir for lcd1602_i2c.h, shared for hw_registers/i2c_driver/delay
INCLUDES := . ../../shared/include

# BSP delay (shared root)
VPATH += ../../shared

# Source files
SRCS = \
	main.c \
	lcd1602_i2c.c \
	util.c \
	i2c_driver.c

# Drivers used (adds drivers directory to VPATH)
DRIVERS = i2c_driver.c

# Include common Makefile
include ../shared/common.mk
