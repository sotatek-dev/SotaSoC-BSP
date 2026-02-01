/*
 * GPIO interrupt example: button (input) toggles LED (output).
 * Button on pin 1, LED on pin 0. Rising edge on button triggers
 * machine external interrupt; handler toggles LED and clears pending.
 * Software debounce ignores repeat edges within DEBOUNCE_MS (get_tick_ms).
 */

#include <stdint.h>

#include "gpio_driver.h"
#include "irq.h"
#include "util.h"

#define LED_PIN      0u
#define BTN_PIN      1u
#define DEBOUNCE_MS  50u

static unsigned long last_btn_tick_ms;

static void gpio_irq_handler(void) {
  if (!(gpio_int_get_pending() & (1u << BTN_PIN)))
    return;

  gpio_int_clear(BTN_PIN);

  unsigned long now = get_tick_ms();
  unsigned long elapsed = now - last_btn_tick_ms;
  if (elapsed < DEBOUNCE_MS)
    return;  /* ignore bounce */

  last_btn_tick_ms = now;
  gpio_toggle_pin(LED_PIN);
}

int main(void) {
  gpio_set_direction(LED_PIN, GPIO_OUTPUT);
  gpio_set_direction(BTN_PIN, GPIO_INPUT);
  gpio_clear_pin(LED_PIN);

  last_btn_tick_ms = get_tick_ms();  /* allow first press immediately */
  gpio_int_enable(BTN_PIN);

  irq_init();
  irq_register(IRQ_MEXT, gpio_irq_handler);

  irq_enable_meie(1);
  irq_global_enable(1);

  for (;;) __asm__ volatile("wfi");
}
