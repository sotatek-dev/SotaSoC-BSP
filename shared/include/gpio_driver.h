#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "hw_registers.h"

// GPIO direction
#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

// GPIO driver return codes
#define GPIO_OK 0
#define GPIO_ERROR -1

// Maximum pin number (0-based)
#define GPIO_PIN_MAX 6

/**
 * @brief Set direction of a single pin
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @param direction GPIO_INPUT or GPIO_OUTPUT
 * @return GPIO_OK on success, GPIO_ERROR if pin out of range
 */
int gpio_set_direction(unsigned int pin, int direction);

/**
 * @brief Set direction of multiple pins at once (mask: 1 = output, 0 = input)
 * @param mask Bitmask for direction (bit N = 1 means pin N is output)
 */
void gpio_set_direction_mask(unsigned int mask);

/**
 * @brief Set pin high (output)
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success
 */
int gpio_set_pin(unsigned int pin);

/**
 * @brief Clear pin low (output)
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success
 */
int gpio_clear_pin(unsigned int pin);

/**
 * @brief Toggle pin output state
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success
 */
int gpio_toggle_pin(unsigned int pin);

/**
 * @brief Write value to a single pin (0 or 1)
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @param value 0 for low, non-zero for high
 * @return GPIO_OK on success
 */
int gpio_write_pin(unsigned int pin, int value);

/**
 * @brief Write to all output pins at once
 * @param value Bitmask for output state (bit N = pin N level)
 */
void gpio_write(unsigned int value);

/**
 * @brief Read state of a single pin (input)
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return 0 or 1, or GPIO_ERROR if pin out of range
 */
int gpio_read_pin(unsigned int pin);

/**
 * @brief Read all input pins at once
 * @return Bitmask of input states
 */
unsigned int gpio_read(void);

/* GPIO interrupt (rising edge per pin) */

/**
 * @brief Enable rising-edge interrupt for a pin
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success, GPIO_ERROR if pin out of range
 */
int gpio_int_enable(unsigned int pin);

/**
 * @brief Disable interrupt for a pin
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success, GPIO_ERROR if pin out of range
 */
int gpio_int_disable(unsigned int pin);

/**
 * @brief Read interrupt pending mask (bit N = 1 if pin N has pending interrupt)
 * @return Pending mask
 */
unsigned int gpio_int_get_pending(void);

/**
 * @brief Clear pending interrupt for a pin (write 1 to clear)
 * @param pin Pin number (0..GPIO_PIN_MAX)
 * @return GPIO_OK on success, GPIO_ERROR if pin out of range
 */
int gpio_int_clear(unsigned int pin);

#endif  // GPIO_DRIVER_H
