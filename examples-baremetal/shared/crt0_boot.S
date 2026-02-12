/* Boot stub: set stack pointer, copy .data from ROM to RAM, then jump to newlib's _start.
 * Linked first when using newlib; ELF entry set to _start_boot so this runs
 * before crt0's _start (which does not set sp and does not copy .data). */

.section .text.start
.globl _start_boot
.type _start_boot, @function

_start_boot:
	la   sp, __stack_top

	/* Copy .data from load address (ROM) to run address (RAM). */
	la   t0, __data_load_start
	la   t1, __data_start
	la   t2, __data_end
1:
	bgeu t1, t2, 2f
	lw   s0, 0(t0)
	sw   s0, 0(t1)
	addi t0, t0, 4
	addi t1, t1, 4
	j    1b
2:
	tail _start

.size _start_boot, . - _start_boot
