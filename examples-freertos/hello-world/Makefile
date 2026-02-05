# FreeRTOS HelloWorld — paths relative to build/gcc/
OUTPUT_DIR := build
IMAGE := hello-world.elf

# Project sets kernel and shared paths (required by freertos-rv32.mk)
KERNEL_DIR = ../../FreeRTOS-Kernel
FREERTOS_SHARED_DIR = ../shared

# App sources and includes (path to this HelloWorld from build/gcc)
APP_DIR = .
VPATH += $(APP_DIR)
INCLUDE_DIRS += -I$(APP_DIR)
SOURCE_FILES += $(APP_DIR)/app_main.c
include ../shared/freertos-rv32.mk
