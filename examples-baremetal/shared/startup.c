// Startup code for bare-metal RISC-V applications
// This file provides the entry point _start() that:
// - Sets stack pointer (sp) and global pointer (gp)
// - Copies .data from ROM (load address) to RAM (run address)
// - Zeros .bss
// - Calls main()

void _start() __attribute__((section(".text.start")));
void _start()
{
  // Set stack and global pointer before any C code uses them
  __asm__ volatile("la sp, __stack_top");
  __asm__ volatile("la gp, __global_pointer$");

  // Copy .data from ROM to RAM (linker places .data in RAM AT > ROM)
  extern char __data_start[] __attribute__((aligned(4)));
  extern char __data_end[] __attribute__((aligned(4)));
  extern char __data_load_start[] __attribute__((aligned(4)));
  for (char *dst = __data_start, *src = __data_load_start; dst < __data_end;)
    *dst++ = *src++;

  // Zero .bss (uninitialized globals)
  extern char __bss_start[] __attribute__((aligned(4)));
  extern char __bss_end[] __attribute__((aligned(4)));
  for (char *p = __bss_start; p < __bss_end;) *p++ = 0;

  __asm__ volatile("call main");

  // Infinite loop if main returns
  for (;;);
}
