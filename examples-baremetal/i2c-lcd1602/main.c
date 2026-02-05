/*
 * Display text on LCD1602 with PCF8574 I2C backpack
 */

#include "hw_registers.h"
#include "i2c_driver.h"
#include "lcd1602_i2c.h"

int main(void) {
  i2c_init(I2C_PRESCALE_100KHZ);

  if (lcd1602_init(LCD1602_I2C_ADDR_DEFAULT) != 0) {
    /* If default 0x27 fails, try 0x3F (common alternate) */
    lcd1602_init(0x3F);
  }

  lcd1602_clear();
  lcd1602_set_position(0, 0);
  lcd1602_print("Hello, Sota-SoC!");
  lcd1602_set_position(0, 1);
  lcd1602_print("SotaTek");

  while (1) {
    __asm__ volatile("nop");
  }
}
