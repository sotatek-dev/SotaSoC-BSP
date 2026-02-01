#include "hw_registers.h"

#define GPIO_DIR_REG (*(volatile unsigned int *)GPIO_DIR)
#define GPIO_OUT_REG (*(volatile unsigned int *)GPIO_OUT)

int main(void) {
  GPIO_DIR_REG = 0xFF;
  GPIO_OUT_REG = 0x00;

  while (1) {
    for (int i = 0; i < 1000000; i++) {
    }
    GPIO_OUT_REG ^= 1u;
  }
}
