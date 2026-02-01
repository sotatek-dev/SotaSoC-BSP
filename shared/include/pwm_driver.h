#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "hw_registers.h"

// PWM channel indices
#define PWM_CHANNEL_0  0
#define PWM_CHANNEL_1  1

// PWM driver return codes
#define PWM_OK    0
#define PWM_ERROR -1

// Hardware limit: period, duty, and counter are 16-bit (0 .. PWM_MAX_PERIOD)
#define PWM_MAX_PERIOD  0xFFFFu

/**
 * @brief Initialize a PWM channel (disable output, zero period/duty)
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @return PWM_OK on success, PWM_ERROR if channel invalid
 */
int pwm_init(unsigned int channel);

/**
 * @brief Set period in clock ticks (total cycle length)
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @param period Number of clock ticks per period (0..PWM_MAX_PERIOD)
 * @return PWM_OK on success, PWM_ERROR if channel or period > PWM_MAX_PERIOD
 */
int pwm_set_period(unsigned int channel, unsigned int period);

/**
 * @brief Set duty cycle in clock ticks (high time)
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @param duty Number of ticks output is high (0..PWM_MAX_PERIOD, should be <= period)
 * @return PWM_OK on success, PWM_ERROR if channel or duty > PWM_MAX_PERIOD
 */
int pwm_set_duty(unsigned int channel, unsigned int duty);

/**
 * @brief Enable PWM output for a channel
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @return PWM_OK on success, PWM_ERROR if channel invalid
 */
int pwm_enable(unsigned int channel);

/**
 * @brief Disable PWM output for a channel
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @return PWM_OK on success, PWM_ERROR if channel invalid
 */
int pwm_disable(unsigned int channel);

/**
 * @brief Configure and start PWM (period, duty, enable)
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @param period Clock ticks per period
 * @param duty Clock ticks high per period (0..PWM_MAX_PERIOD)
 * @return PWM_OK on success, PWM_ERROR if channel or period/duty > PWM_MAX_PERIOD
 */
int pwm_start(unsigned int channel, unsigned int period, unsigned int duty);

/**
 * @brief Read current counter value (for debugging)
 * @param channel PWM_CHANNEL_0 or PWM_CHANNEL_1
 * @return Counter value (0..PWM_MAX_PERIOD), or 0 if channel invalid
 */
unsigned int pwm_get_counter(unsigned int channel);

#endif  // PWM_DRIVER_H
