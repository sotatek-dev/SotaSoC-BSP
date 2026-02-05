/*
 * LCD1602 I2C driver for PCF8574 backpack + HD44780 16x2 LCD
 * Ported from Bonezegei LCD1602 I2C (Arduino) to C
 */

#include "lcd1602_i2c.h"

#include "i2c_driver.h"
#include "util.h"

/* PCF8574 pin mapping: bit0=RS, bit1=RW, bit2=E, bit3=Backlight, bit4-7=D4-D7
 */
#define LCD_RS 0x01
#define LCD_E 0x04
#define LCD_BL 0x08

static unsigned char lcd_addr;
/* current byte to PCF8574 (RS,RW,E,BL in low nibble) */
static unsigned char p_write;

static void lcd_write(unsigned char data) { i2c_write_byte(lcd_addr, data); }

static void lcd_latch(void) {
  p_write &= ~LCD_E;
  lcd_write(p_write);
  delay_ms(1);
  p_write |= LCD_E;
  lcd_write(p_write);
  delay_ms(1);
  p_write &= ~LCD_E;
  lcd_write(p_write);
  delay_us(100);
}

static void lcd_write4bit(unsigned char data) {
  p_write &= 0x0F;
  p_write |= (data << 4);
  lcd_latch();
}

static void lcd_write_byte(unsigned char data) {
  lcd_write4bit(data >> 4);
  lcd_write4bit(data & 0x0F);
}

static void lcd_write_command(unsigned char data) {
  p_write &= ~LCD_RS;
  lcd_write(p_write);
  lcd_write_byte(data);
}

static void lcd_write_data(unsigned char data) {
  p_write |= LCD_RS;
  lcd_write(p_write);
  lcd_write_byte(data);
}

int lcd1602_init(unsigned char addr) {
  lcd_addr = addr;
  p_write = 0;

  lcd_write(0x00);
  lcd_write4bit(0x03);
  delay_us(4500);
  lcd_write4bit(0x03);
  delay_us(4500);
  lcd_write4bit(0x03);
  delay_us(150);
  lcd_write4bit(0x02);

  lcd_write4bit(0x02);
  lcd_write4bit(0x0C);
  lcd_write4bit(0x00);
  lcd_write4bit(0x08);
  lcd_write4bit(0x00);
  lcd_write4bit(0x01);
  lcd_write4bit(0x00);
  lcd_write4bit(0x06);

  lcd_write4bit(0x00);
  lcd_write4bit(0x0F);
  lcd1602_set_backlight(1);

  return 0;
}

void lcd1602_clear(void) { lcd_write_command(0x01); }

void lcd1602_set_position(unsigned char x, unsigned char y) {
  if (y == 0)
    lcd_write_command(0x80 | x);
  else if (y == 1)
    lcd_write_command(0x80 | 0x40 | x);
}

void lcd1602_print(const char *str) {
  while (*str) {
    lcd_write_data((unsigned char)*str);
    str++;
  }
}

void lcd1602_print_char(char ch) { lcd_write_data((unsigned char)ch); }

void lcd1602_set_backlight(unsigned char on) {
  p_write = 0;
  if (on) {
    p_write |= LCD_BL;
    lcd_write(p_write);
  } else {
    p_write &= ~LCD_BL;
    lcd_write(p_write);
  }
  delay_ms(50);
}
