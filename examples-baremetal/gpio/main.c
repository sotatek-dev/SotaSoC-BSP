#include "gpio_driver.h"

// GPIO example: demonstrates init, direction, output (blink), and read API

int main(void) {
  // Pins 0-7 as output, rest as input
  gpio_set_direction_mask(0xFF);
  gpio_write(0x00);

  while (1)
  {
    // Blink pin 0
    for (volatile int i = 0; i < 100000; i++) {
    }
    gpio_toggle_pin(0);

    // Optional: cycle through pins 1-3 (uncomment to try)
    // gpio_toggle_pin(1);
    // gpio_toggle_pin(2);
    // gpio_toggle_pin(3);

    // Read back input pins (e.g. if pins 8+ are wired as input)
    // unsigned int inputs = gpio_read();
  }
}
