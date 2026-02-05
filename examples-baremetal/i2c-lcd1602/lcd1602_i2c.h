/*
 * LCD1602 I2C driver for PCF8574 backpack + HD44780 16x2 LCD
 * Ported from Bonezegei LCD1602 I2C (Arduino) to C
 */

#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

/* Default PCF8574 I2C address (A0=A1=A2=0) */
#define LCD1602_I2C_ADDR_DEFAULT 0x27

/**
 * @brief Initialize LCD1602 over I2C (4-bit mode, 2 lines, 5x8)
 * @param addr 7-bit I2C address of PCF8574 (e.g. 0x27)
 * @return 0 on success, negative on I2C error
 */
int lcd1602_init(unsigned char addr);

/**
 * @brief Clear display and home cursor
 */
void lcd1602_clear(void);

/**
 * @brief Set cursor position
 * @param x column 0..15
 * @param y row 0 or 1
 */
void lcd1602_set_position(unsigned char x, unsigned char y);

/**
 * @brief Print a null-terminated string
 */
void lcd1602_print(const char *str);

/**
 * @brief Print a single character
 */
void lcd1602_print_char(char ch);

/**
 * @brief Turn backlight on (1) or off (0)
 */
void lcd1602_set_backlight(unsigned char on);

#endif /* LCD1602_I2C_H */
