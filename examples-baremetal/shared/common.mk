# Common Makefile for bare-metal examples
# Include this file in each example's Makefile

# Toolchain (force set, don't allow override from environment)
CC := riscv32-unknown-elf-gcc
LD := riscv32-unknown-elf-ld
OCPY := riscv32-unknown-elf-objcopy
ODUMP := riscv32-unknown-elf-objdump
SZ := riscv32-unknown-elf-size
MKDIR := mkdir

# Project name must be defined before including this file
ifndef PROJECT
$(error PROJECT is not set. Please define PROJECT in your Makefile)
endif

# Build directory
BUILD_DIR ?= build
OBJ_DIR = $(BUILD_DIR)/objs/a/b/c

# Compiler flags
CFLAGS += -Wall -Werror -std=c11 -O3 -ffreestanding \
          -ffunction-sections -fdata-sections \
          -march=rv32ec_zicsr -mabi=ilp32e

# Source files must be defined before including this file
ifndef SRCS
$(error SRCS is not set. Please define SRCS in your Makefile)
endif

# Use libc (newlib) by default. Set USE_STDLIB=0 for bare-metal (nostdlib + startup.c)
ifeq ($(USE_STDLIB),0)
CFLAGS += -nostdlib
LDFLAGS += -lgcc
SRCS += startup.c
else
SRCS += syscalls.c
# Boot stub runs first: set sp then jump to newlib's _start (which does not set sp)
BOOT_OBJ := $(OBJ_DIR)/crt0_boot.o
LDFLAGS += -Wl,-e,_start_boot
endif

# Linker script (can be overridden)
LINKER_SCRIPT ?= ../shared/linker.ld

# Linker flags
LDFLAGS += \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(PROJECT).map \
	-Wl,-T,$(LINKER_SCRIPT) \
	-march=rv32ec_zicsr \
	-mabi=ilp32e

# Include paths (can be overridden in individual Makefiles)
INCLUDES ?= ../../shared/include
CFLAGS += $(foreach i,$(INCLUDES),-I$(i))

# Defines (can be overridden in individual Makefiles)
DEFINES ?=
CFLAGS += $(foreach d,$(DEFINES),-D$(d))

# BSP shared (trap_handler.S) and drivers (irq.c)
BSP_SHARED ?= ../../shared
# Driver directory (optional, for projects using drivers)
DRIVER_DIR ?= ../../shared/drivers
ifdef DRIVERS
VPATH += $(DRIVER_DIR)
endif

# IRQ subsystem
SRCS += irq.c

# Shared directory (for startup code)
SHARED_DIR ?= ../shared
VPATH += $(SHARED_DIR)

# Object files
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS += $(OBJ_DIR)/trap_handler.o
ifneq ($(USE_STDLIB),0)
OBJS := $(BOOT_OBJ) $(OBJS)
endif

# Default target
.PHONY: all
all: $(BUILD_DIR)/$(PROJECT).bin

# Create build directories
$(BUILD_DIR):
	$(NO_ECHO)$(MKDIR) -p $(BUILD_DIR)

$(OBJ_DIR):
	$(NO_ECHO)$(MKDIR) -p $(OBJ_DIR)

# Boot stub (newlib only): set sp then jump to _start
$(OBJ_DIR)/crt0_boot.o: crt0_boot.S $(OBJ_DIR)
	@echo "Assembling $<"
	$(NO_ECHO)$(CC) $(CFLAGS) -c -o $@ $<

# Compile C files
$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	@echo "Compiling $<"
	$(NO_ECHO)$(MKDIR) -p $(dir $@)
	$(NO_ECHO)$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)/irq.o: $(DRIVER_DIR)/irq.c | $(OBJ_DIR)
	@echo "Compiling $<"
	$(NO_ECHO)$(MKDIR) -p $(dir $@)
	$(NO_ECHO)$(CC) -c -o $@ $< $(CFLAGS)

# Generate listing file
$(BUILD_DIR)/$(PROJECT).lst: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)
	$(ODUMP) -D $< > $@

# Generate binary file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).lst
	$(OCPY) $< $@ -O binary
	$(SZ) $<

# Trap handler (single entry for all exceptions/interrupts)
$(OBJ_DIR)/trap_handler.o: $(BSP_SHARED)/trap_handler.S | $(OBJ_DIR)
	@echo "Assembling $<"
	$(NO_ECHO)$(CC) $(CFLAGS) -c -o $@ $<

# Link object files
$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@echo "Linking $@"
	$(NO_ECHO)$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Print variables (for debugging)
.PHONY: print-vars
print-vars:
	@echo "PROJECT: $(PROJECT)"
	@echo "SRCS: $(SRCS)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "DRIVERS: $(DRIVERS)"
	@echo "BUILD_DIR: $(BUILD_DIR)"
	@echo "OBJ_DIR: $(OBJ_DIR)"
