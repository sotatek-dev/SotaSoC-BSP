# Common Makefile fragment for FreeRTOS RV32E projects
# Before including: set KERNEL_DIR, FREERTOS_SHARED_DIR, OUTPUT_DIR, IMAGE,
# then VPATH, INCLUDE_DIRS, SOURCE_FILES for the app (must implement vApplicationStart()).

# Linker script in shared; override in project if needed
LINKER_SCRIPT ?= $(FREERTOS_SHARED_DIR)/linker.ld

# --- Toolchain (add toolchain bin to PATH; override: make CROSS_COMPILE=...) ---
CROSS_COMPILE ?= riscv32-unknown-elf-

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# Object files in separate folder (like bare-metal)
OBJ_DIR = $(OUTPUT_DIR)/objs

# Architecture
MARCH ?= rv32e_zicsr_zifencei
MABI ?= ilp32e
MCMODEL ?= medlow
$(info  Using RV32 build with zicsr)

KERNEL_PORT_DIR = $(KERNEL_DIR)/portable/GCC/RISC-V

# Includes for kernel, port, and shared (riscv-reg.h for start.S)
INCLUDE_DIRS += -I$(KERNEL_PORT_DIR)/chip_specific_extensions/RV32I_CLINT_no_extensions
INCLUDE_DIRS += -I$(KERNEL_DIR)/include -I$(KERNEL_PORT_DIR) -I$(FREERTOS_SHARED_DIR)

# Compiler flags
CFLAGS += $(INCLUDE_DIRS) -fmessage-length=0 \
          -march=$(MARCH) -mabi=$(MABI) -mcmodel=$(MCMODEL) -ffunction-sections -fdata-sections \
          -Wno-unused-parameter -nostartfiles -g3 -O0

ifeq ($(PICOLIBC),1)
  CFLAGS += --specs=picolibc.specs -DPICOLIBC_INTEGER_PRINTF_SCANF
else
  CFLAGS += --specs=nano.specs -fno-builtin-printf
endif

# Linker flags (LINKER_SCRIPT and MAP set by project or default)
MAP_FILE ?= $(OUTPUT_DIR)/$(IMAGE:.elf=.map)
LDFLAGS += -nostartfiles -Xlinker --gc-sections -Wl,-Map,$(MAP_FILE) \
           -T$(LINKER_SCRIPT) -march=$(MARCH) -mabi=$(MABI) -mcmodel=$(MCMODEL) -Xlinker \
           --defsym=__stack_size=352 -Wl,--start-group -Wl,--end-group

ifeq ($(PICOLIBC),1)
  LDFLAGS += --specs=picolibc.specs --oslib=semihost --crt0=minimal -DPICOLIBC_INTEGER_PRINTF_SCANF
else
  LDFLAGS += -Wl,--wrap=malloc -Wl,--wrap=free -Wl,--wrap=open -Wl,--wrap=lseek -Wl,--wrap=read -Wl,--wrap=write \
             -Wl,--wrap=fstat -Wl,--wrap=stat -Wl,--wrap=close -Wl,--wrap=link -Wl,--wrap=unlink \
             -Wl,--wrap=execve -Wl,--wrap=fork -Wl,--wrap=getpid -Wl,--wrap=kill -Wl,--wrap=wait \
             -Wl,--wrap=isatty -Wl,--wrap=times -Wl,--wrap=sbrk -Wl,--wrap=_malloc \
             -Wl,--wrap=_free -Wl,--wrap=_open -Wl,--wrap=_lseek -Wl,--wrap=_read -Wl,--wrap=_write \
             -Wl,--wrap=_fstat -Wl,--wrap=_stat -Wl,--wrap=_close -Wl,--wrap=_link -Wl,--wrap=_unlink \
             -Wl,--wrap=_execve -Wl,--wrap=_fork -Wl,--wrap=_getpid -Wl,--wrap=_kill -Wl,--wrap=_wait \
             -Wl,--wrap=_isatty -Wl,--wrap=_times -Wl,--wrap=_sbrk -Wl,--wrap=__exit
endif

# Kernel and shared (start.S) sources and VPATH
VPATH += $(KERNEL_DIR) $(KERNEL_PORT_DIR) $(KERNEL_DIR)/portable/MemMang $(FREERTOS_SHARED_DIR)
SOURCE_FILES += $(KERNEL_DIR)/tasks.c $(KERNEL_DIR)/list.c $(KERNEL_DIR)/queue.c \
                $(KERNEL_DIR)/timers.c $(KERNEL_DIR)/event_groups.c $(KERNEL_DIR)/stream_buffer.c
SOURCE_FILES += $(KERNEL_DIR)/portable/MemMang/heap_4.c
SOURCE_FILES += $(KERNEL_DIR)/portable/GCC/RISC-V/port.c
ASM_SOURCE_FILES += $(KERNEL_DIR)/portable/GCC/RISC-V/portASM.S
ASM_SOURCE_FILES += $(FREERTOS_SHARED_DIR)/start.S
# Lightweight printf when not using picolibc (weak putchar; override in project if needed)
ifneq ($(PICOLIBC),1)
SOURCE_FILES += $(FREERTOS_SHARED_DIR)/printf-stdarg.c
endif
# Common entry + hooks (calls vApplicationStart(); project must implement it)
SOURCE_FILES += $(FREERTOS_SHARED_DIR)/freertos_main.c

# App: project adds VPATH, INCLUDE_DIRS, SOURCE_FILES for vApplicationStart() and app code

# Object and dependency lists (recursive = so they see SOURCE_FILES/ASM added after include)
OBJS = $(SOURCE_FILES:%.c=%.o) $(ASM_SOURCE_FILES:%.S=%.o)
OBJS_NO_PATH = $(notdir $(OBJS))
OBJS_OUTPUT = $(OBJS_NO_PATH:%.o=$(OBJ_DIR)/%.o)

DEP_FILES_NO_PATH = $(OBJS_NO_PATH:%.o=%.d)
DEP_OUTPUT = $(DEP_FILES_NO_PATH:%.d=$(OBJ_DIR)/%.d)

# Outputs: .elf, .bin, .lst (like bare-metal)
ELF_FILE = $(OUTPUT_DIR)/$(IMAGE)
BIN_FILE = $(OUTPUT_DIR)/$(IMAGE:.elf=.bin)
LST_FILE = $(OUTPUT_DIR)/$(IMAGE:.elf=.lst)

# Targets
all: $(ELF_FILE) $(BIN_FILE) $(LST_FILE)

# Create obj dir
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Include .d files (after first build they exist and add prerequisites)
include $(wildcard $(DEP_OUTPUT))

# Keep .d files so make does not remove them as intermediates
.SECONDARY: $(DEP_OUTPUT)

# Allow .d to be "built" when missing so .o rule can run and create it via -MMD -MP -MF
$(OBJ_DIR)/%.d: ;

$(OBJ_DIR)/%.o : %.c $(OBJ_DIR)/%.d Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -MF $(OBJ_DIR)/$*.d -c $< -o $@

$(OBJ_DIR)/%.o: %.S $(OBJ_DIR)/%.d Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -MF $(OBJ_DIR)/$*.d -c $< -o $@

$(ELF_FILE): $(OBJS_OUTPUT) Makefile
	@echo ""
	@echo "--- Final linking ---"
	$(LD) $(OBJS_OUTPUT) $(LDFLAGS) -o $(ELF_FILE)
	$(SIZE) $(ELF_FILE)

$(LST_FILE): $(ELF_FILE)
	$(OBJDUMP) -D $< > $@

$(BIN_FILE): $(ELF_FILE) $(LST_FILE)
	$(OBJCOPY) $< $@ -O binary

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(ELF_FILE) $(BIN_FILE) $(LST_FILE) $(OUTPUT_DIR)/$(IMAGE:.elf=.map)
	rm -f $(OUTPUT_DIR)/*.o $(OUTPUT_DIR)/*.d

print-%  : ; @echo $* = $($*)

.PHONY: all clean
