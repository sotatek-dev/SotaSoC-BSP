/*
 * Blink LED using machine timer interrupt.
 * User registers a timer handler that toggles GPIO and schedules the next
 * mtimecmp.
 */

#include <stdint.h>

#include "hw_registers.h"
#include "irq.h"

#define TIMER_TICKS_PER_BLINK (CLK_HZ / 2) /* ~0.5 s @ 64 MHz */

#define GPIO_DIR_REG (*(volatile unsigned int *)GPIO_DIR)
#define GPIO_OUT_REG (*(volatile unsigned int *)GPIO_OUT)
#define TIMER_MTIME_LO_REG (*(volatile unsigned int *)TIMER_MTIME_LO)
#define TIMER_MTIME_HI_REG (*(volatile unsigned int *)TIMER_MTIME_HI)
#define TIMER_MTIMECMP_LO_REG (*(volatile unsigned int *)TIMER_MTIMECMP_LO)
#define TIMER_MTIMECMP_HI_REG (*(volatile unsigned int *)TIMER_MTIMECMP_HI)

/* Schedule next timer interrupt: mtimecmp = mtime + TIMER_TICKS_PER_BLINK.
 * RV32: write order to avoid spurious interrupt: -1 to lo, then hi, then lo. */
static void schedule_next_timer(void) {
  uint32_t lo = TIMER_MTIME_LO_REG;
  uint32_t hi = TIMER_MTIME_HI_REG;
  uint64_t mtime = ((uint64_t)hi << 32) | (uint64_t)lo;
  uint64_t next = mtime + (uint64_t)TIMER_TICKS_PER_BLINK;

  TIMER_MTIMECMP_LO_REG = (uint32_t)-1;
  TIMER_MTIMECMP_HI_REG = (uint32_t)(next >> 32);
  TIMER_MTIMECMP_LO_REG = (uint32_t)(next & 0xFFFFFFFFu);
}

static void timer_irq_handler(void) {
  GPIO_OUT_REG ^= 1u;
  schedule_next_timer();
}

int main(void) {
  GPIO_DIR_REG = 0xFF;
  GPIO_OUT_REG = 0x00;

  irq_init();
  irq_register(IRQ_MTIMER, timer_irq_handler);
  schedule_next_timer();

  irq_enable_mtie(1);
  irq_global_enable(1);

  for (;;) __asm__ volatile("wfi");
}
