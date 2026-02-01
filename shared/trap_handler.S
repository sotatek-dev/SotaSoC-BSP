/* Trap handler for RV32E.
 * Saves caller/callee regs, calls irq_trap_dispatch(mcause), restores and mret.
 * Frame: 64 bytes (16-byte aligned). */

.section .text
.align 2
.globl trap_entry
.type trap_entry, @function

trap_entry:
    addi  sp, sp, -64
    sw    ra,  0(sp)
    sw    gp,  4(sp)
    sw    tp,  8(sp)
    sw    t0,  12(sp)
    sw    t1,  16(sp)
    sw    t2,  20(sp)
    sw    s0,  24(sp)
    sw    s1,  28(sp)
    sw    a0,  32(sp)
    sw    a1,  36(sp)
    sw    a2,  40(sp)
    sw    a3,  44(sp)
    sw    a4,  48(sp)
    sw    a5,  52(sp)

    csrr  a0, mcause
    call  irq_trap_dispatch

    lw    ra,  0(sp)
    lw    gp,  4(sp)
    lw    tp,  8(sp)
    lw    t0,  12(sp)
    lw    t1,  16(sp)
    lw    t2,  20(sp)
    lw    s0,  24(sp)
    lw    s1,  28(sp)
    lw    a0,  32(sp)
    lw    a1,  36(sp)
    lw    a2,  40(sp)
    lw    a3,  44(sp)
    lw    a4,  48(sp)
    lw    a5,  52(sp)
    addi  sp, sp, 64
    mret

.size trap_entry, . - trap_entry
