OUTPUT_DIR := ./output
IMAGE := RTOSDemo.elf

# The directory that contains the /source and /demo sub directories.
FREERTOS_ROOT = ./../../../..

# Toolchain paths (User specific)
CC = $(COMPILER_PATH)/riscv-none-elf-gcc
LD = $(COMPILER_PATH)/riscv-none-elf-gcc
SIZE = $(COMPILER_PATH)/riscv-none-elf-size
MAKE = make

# Generate GCC_VERSION in number format (Simplified for stability)
GCC_VERSION = $(shell $(CC) --version | grep ^$(CC) | sed 's/^.* //g' | awk -F. '{ printf("%d%02d%02d"), $$1, $$2, $$3 }')

# Architecture Setup
MARCH=rv32e_zicsr_zifencei
MABI=ilp32e
MCMODEL=medlow
$(info  Using RV32 build with zicsr)

INCLUDE_DIRS += -I$(KERNEL_PORT_DIR)/chip_specific_extensions/RV32I_CLINT_no_extensions

# Compiler Flags
CFLAGS += $(INCLUDE_DIRS) -fmessage-length=0 \
          -march=$(MARCH) -mabi=$(MABI) -mcmodel=$(MCMODEL) -ffunction-sections -fdata-sections \
          -Wno-unused-parameter -nostartfiles -g3 -O0

ifeq ($(PICOLIBC),1)
  CFLAGS += --specs=picolibc.specs -DPICOLIBC_INTEGER_PRINTF_SCANF
else
  # FIX: Restore nano.specs for standard GCC usage
  CFLAGS += --specs=nano.specs -fno-builtin-printf
endif

# Linker Flags
LDFLAGS += -nostartfiles -Xlinker --gc-sections -Wl,-Map,$(OUTPUT_DIR)/RTOSDemo.map \
           -T./linker.ld -march=$(MARCH) -mabi=$(MABI) -mcmodel=$(MCMODEL) -Xlinker \
           --defsym=__stack_size=352 -Wl,--start-group -Wl,--end-group

ifeq ($(PICOLIBC),1)
  LDFLAGS += --specs=picolibc.specs --oslib=semihost --crt0=minimal -DPICOLIBC_INTEGER_PRINTF_SCANF
else
  # FIX: Removed puts and _puts from the wrap list below to avoid linker errors
  LDFLAGS +=  -Wl,--wrap=malloc \
             -Wl,--wrap=free -Wl,--wrap=open -Wl,--wrap=lseek -Wl,--wrap=read -Wl,--wrap=write \
             -Wl,--wrap=fstat -Wl,--wrap=stat -Wl,--wrap=close -Wl,--wrap=link -Wl,--wrap=unlink \
             -Wl,--wrap=execve -Wl,--wrap=fork -Wl,--wrap=getpid -Wl,--wrap=kill -Wl,--wrap=wait \
             -Wl,--wrap=isatty -Wl,--wrap=times -Wl,--wrap=sbrk -Wl,--wrap=_malloc \
             -Wl,--wrap=_free -Wl,--wrap=_open -Wl,--wrap=_lseek -Wl,--wrap=_read -Wl,--wrap=_write \
             -Wl,--wrap=_fstat -Wl,--wrap=_stat -Wl,--wrap=_close -Wl,--wrap=_link -Wl,--wrap=_unlink \
             -Wl,--wrap=_execve -Wl,--wrap=_fork -Wl,--wrap=_getpid -Wl,--wrap=_kill -Wl,--wrap=_wait \
             -Wl,--wrap=_isatty -Wl,--wrap=_times -Wl,--wrap=_sbrk -Wl,--wrap=__exit
endif

#
# Kernel build.
#
KERNEL_DIR = $(FREERTOS_ROOT)/Kernel
KERNEL_PORT_DIR += $(KERNEL_DIR)/portable/GCC/RISC-V
INCLUDE_DIRS += -I$(KERNEL_DIR)/include \
                -I$(KERNEL_PORT_DIR)
VPATH += $(KERNEL_DIR) $(KERNEL_PORT_DIR) $(KERNEL_DIR)/portable/MemMang
SOURCE_FILES += $(KERNEL_DIR)/tasks.c
SOURCE_FILES += $(KERNEL_DIR)/list.c
SOURCE_FILES += $(KERNEL_DIR)/queue.c
SOURCE_FILES += $(KERNEL_DIR)/timers.c
SOURCE_FILES += $(KERNEL_DIR)/event_groups.c
SOURCE_FILES += $(KERNEL_DIR)/stream_buffer.c
SOURCE_FILES += $(KERNEL_DIR)/portable/MemMang/heap_4.c
SOURCE_FILES += $(KERNEL_DIR)/portable/GCC/RISC-V/port.c
ASM_SOURCE_FILES += $(KERNEL_DIR)/portable/GCC/RISC-V/portASM.S

#
# Common demo files
#
DEMO_ROOT = $(FREERTOS_ROOT)/Demo

#
# Application entry point.
#
DEMO_PROJECT = $(DEMO_ROOT)/HelloWorld
VPATH += $(DEMO_PROJECT)
INCLUDE_DIRS += -I$(DEMO_PROJECT)

SOURCE_FILES += $(DEMO_PROJECT)/main.c
SOURCE_FILES += $(DEMO_PROJECT)/main_real.c

# Lightweight print formatting
ifneq ($(PICOLIBC),1)
SOURCE_FILES += ./printf-stdarg.c
endif

ASM_SOURCE_FILES += ./start.S

# Create a list of object files
OBJS = $(SOURCE_FILES:%.c=%.o) $(ASM_SOURCE_FILES:%.S=%.o)
OBJS_NO_PATH = $(notdir $(OBJS))
OBJS_OUTPUT = $(OBJS_NO_PATH:%.o=$(OUTPUT_DIR)/%.o)

# Create a list of dependency files
DEP_FILES := $(SOURCE_FILES:%.c=$(OUTPUT_DIR)/%.d) $(ASM_SOURCE_FILES:%.S=$(OUTPUT_DIR)/%.d)
DEP_FILES_NO_PATH = $(notdir $(DEP_FILES))
DEP_OUTPUT = $(DEP_FILES_NO_PATH:%.d=$(OUTPUT_DIR)/%.d)

all: $(OUTPUT_DIR)/$(IMAGE)

%.o : %.c
$(OUTPUT_DIR)/%.o : %.c $(OUTPUT_DIR)/%.d Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o : %.S
$(OUTPUT_DIR)/%.o: %.S $(OUTPUT_DIR)/%.d Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OUTPUT_DIR)/$(IMAGE): $(OBJS_OUTPUT) Makefile
	@echo ""
	@echo ""
	@echo "--- Final linking ---"
	@echo ""
	$(LD) $(OBJS_OUTPUT) $(LDFLAGS) -o $(OUTPUT_DIR)/$(IMAGE)
	$(SIZE) $(OUTPUT_DIR)/$(IMAGE)

$(DEP_OUTPUT):
include $(wildcard $(DEP_OUTPUT))

clean:
	rm -f $(OUTPUT_DIR)/$(IMAGE) $(OUTPUT_DIR)/*.o $(OUTPUT_DIR)/*.d $(OUTPUT_DIR)/*.map

print-%  : ; @echo $* = $($*)

.PHONY: all clean
