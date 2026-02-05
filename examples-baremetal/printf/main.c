#include <stdio.h>

#include "gpio_driver.h"

int main(void) {
  gpio_set_direction_mask(0xFF);
  gpio_write(0x00);

  printf("Hello, SotaSoC!\n");

  while (1) {
    for (volatile int i = 0; i < 100000; ++i) {
    }

    gpio_toggle_pin(0);

    printf("Hello, SotaTek!\n");
  }
}
