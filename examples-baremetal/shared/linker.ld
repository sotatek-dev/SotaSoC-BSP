ENTRY(_start)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 0x01000000
    RAM (rw)  : ORIGIN = 0x01000000, LENGTH = 0x01000000
}

/* Explicit LOAD segments: RX for code, RW for data*/
PHDRS
{
    rom PT_LOAD FLAGS(5); /* PF_R | PF_X = 5 */
    ram PT_LOAD FLAGS(6); /* PF_R | PF_W = 6 */
}

SECTIONS
{
    .text : {
        KEEP(*(.text.start))
        *(.text)
        *(.text.*)
    } > ROM :rom

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > ROM :rom

    .preinit_array : { KEEP(*(.preinit_array)) } > ROM :rom
    .init_array    : { KEEP(*(.init_array)) KEEP(*(.init_array.*)) } > ROM :rom
    .fini_array    : { KEEP(*(.fini_array)) KEEP(*(.fini_array.*)) } > ROM :rom

    /* Global pointer for RISC-V small data (sdata/sbss) */
    __global_pointer$ = ORIGIN(RAM) + 0x800;

    .data : ALIGN(4) {
        __data_start = .;
        *(.data)
        *(.data.*)
        *(.sdata)
        *(.sdata.*)
        __data_end = .;
    } > RAM AT > ROM :ram

    __data_load_start = LOADADDR(.data);

    .bss : ALIGN(4) {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        *(.sbss)
        *(.sbss.*)
        __bss_end = .;
    } > RAM :ram

    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
    /* Heap must not grow past this (leave space for stack) */
    __heap_limit = __stack_top - 0x1000;

    /* End of BSS / start of heap (for future sbrk) */
    _end = .;

    /DISCARD/ : {
        *(.note.GNU-stack)
        *(.gnu_debuglink)
        *(.gnu.lto_*)
    }
}