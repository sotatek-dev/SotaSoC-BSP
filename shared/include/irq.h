#ifndef IRQ_H
#define IRQ_H

#include <stddef.h>

/* RISC-V machine interrupt numbers (same as mcause when bit 31 = 1) */
#define IRQ_MTIMER  7   /* Machine timer interrupt */
#define IRQ_MEXT    11  /* Machine external interrupt */
#define IRQ_MAX     16  /* Max standard IRQ number (0..15) */

typedef void (*irq_handler_t)(void);

/* Initialize interrupt subsystem: set mtvec, clear handler table.
 * Call once before registering handlers or enabling interrupts. */
void irq_init(void);

/* Register a handler for the given IRQ number (e.g. IRQ_MTIMER).
 * Returns 0 on success, -1 if irq_num >= IRQ_MAX. */
int irq_register(unsigned irq_num, irq_handler_t handler);

/* Enable/disable machine timer interrupt (MTIE in mie). */
void irq_enable_mtie(int enable);

/* Enable/disable machine external interrupt (MEIE in mie). */
void irq_enable_meie(int enable);

/* Enable/disable global machine interrupts (MIE in mstatus). */
void irq_global_enable(int enable);

#endif /* IRQ_H */
