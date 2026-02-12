/* Trap handler for RV32I.
 * Saves caller/callee regs, calls irq_trap_dispatch(mcause), restores and mret.
 * Frame: 128 bytes (16-byte aligned). */

.section .text
.align 2
.globl trap_entry
.type trap_entry, @function

trap_entry:
    addi  sp, sp, -128
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
    sw    a6,  56(sp)
    sw    a7,  60(sp)
    sw    t3,  64(sp)
    sw    t4,  68(sp)
    sw    t5,  72(sp)
    sw    t6,  76(sp)
    sw    s2,  80(sp)
    sw    s3,  84(sp)
    sw    s4,  88(sp)
    sw    s5,  92(sp)
    sw    s6,  96(sp)
    sw    s7,  100(sp)
    sw    s8,  104(sp)
    sw    s9,  108(sp)
    sw    s10, 112(sp)
    sw    s11, 116(sp)

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
    lw    a6,  56(sp)
    lw    a7,  60(sp)
    lw    t3,  64(sp)
    lw    t4,  68(sp)
    lw    t5,  72(sp)
    lw    t6,  76(sp)
    lw    s2,  80(sp)
    lw    s3,  84(sp)
    lw    s4,  88(sp)
    lw    s5,  92(sp)
    lw    s6,  96(sp)
    lw    s7,  100(sp)
    lw    s8,  104(sp)
    lw    s9,  108(sp)
    lw    s10, 112(sp)
    lw    s11, 116(sp)
    addi  sp, sp, 128
    mret

.size trap_entry, . - trap_entry
