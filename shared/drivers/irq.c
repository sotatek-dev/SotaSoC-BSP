/* Interrupt: user-registered handlers and mtvec dispatcher. */

#include "irq.h"

#include "hw_registers.h"

#define MCAUSE_INTERRUPT_BIT (1u << 31)
#define MCAUSE_CODE_MASK 0x7FFu
#define MSTATUS_MIE (1u << 3)
#define MIE_MTIE (1u << 7)
#define MIE_MEIE (1u << 11)

extern void trap_entry(void);

static irq_handler_t s_handlers[IRQ_MAX];

void irq_init(void) {
  for (unsigned i = 0; i < IRQ_MAX; i++) s_handlers[i] = (irq_handler_t)0;
  /* mtvec: direct mode (bit 0 = 0), base = trap_entry */
  __asm__ volatile("csrw mtvec, %0" : : "r"((unsigned long)trap_entry));
}

int irq_register(unsigned irq_num, irq_handler_t handler) {
  if (irq_num >= IRQ_MAX) return -1;
  s_handlers[irq_num] = handler;
  return 0;
}

void irq_enable_mtie(int enable) {
  unsigned long mie;
  __asm__ volatile("csrr %0, mie" : "=r"(mie));
  if (enable)
    mie |= MIE_MTIE;
  else
    mie &= ~MIE_MTIE;
  __asm__ volatile("csrw mie, %0" : : "r"(mie));
}

void irq_enable_meie(int enable) {
  unsigned long mie;
  __asm__ volatile("csrr %0, mie" : "=r"(mie));
  if (enable)
    mie |= MIE_MEIE;
  else
    mie &= ~MIE_MEIE;
  __asm__ volatile("csrw mie, %0" : : "r"(mie));
}

void irq_global_enable(int enable) {
  unsigned long mstatus;
  __asm__ volatile("csrr %0, mstatus" : "=r"(mstatus));
  if (enable)
    mstatus |= MSTATUS_MIE;
  else
    mstatus &= ~MSTATUS_MIE;
  __asm__ volatile("csrw mstatus, %0" : : "r"(mstatus));
}

/* Called from trap_entry with mcause in a0. Dispatches to registered handler.
 */
void irq_trap_dispatch(unsigned long mcause) {
  int is_interrupt = (mcause & MCAUSE_INTERRUPT_BIT) != 0;
  unsigned code = (unsigned)(mcause & MCAUSE_CODE_MASK);

  if (!is_interrupt) return; /* exceptions: optional future handling */

  if (code >= IRQ_MAX) return;
  irq_handler_t fn = s_handlers[code];
  if (fn) fn();
}
