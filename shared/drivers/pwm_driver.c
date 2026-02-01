#include "pwm_driver.h"

#define PWM_CHANNEL_MAX 1
#define PWM_16BIT_MASK  0xFFFFu

#define PWM_CTRL(ch)    (*(volatile unsigned int *)(PWM_BASE_ADDR + 0x00 + (ch) * 0x10))
#define PWM_PERIOD(ch)  (*(volatile unsigned int *)(PWM_BASE_ADDR + 0x04 + (ch) * 0x10))
#define PWM_DUTY(ch)    (*(volatile unsigned int *)(PWM_BASE_ADDR + 0x08 + (ch) * 0x10))
#define PWM_COUNTER(ch) (*(volatile unsigned int *)(PWM_BASE_ADDR + 0x0C + (ch) * 0x10))

static int pwm_check_channel(unsigned int channel) {
  if (channel > PWM_CHANNEL_MAX) {
    return PWM_ERROR;
  }
  return PWM_OK;
}

int pwm_init(unsigned int channel) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  PWM_CTRL(channel) = 0;
  PWM_PERIOD(channel) = 0;
  PWM_DUTY(channel) = 0;
  return PWM_OK;
}

int pwm_set_period(unsigned int channel, unsigned int period) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  if (period > PWM_MAX_PERIOD) {
    return PWM_ERROR;
  }
  PWM_PERIOD(channel) = period & PWM_16BIT_MASK;
  return PWM_OK;
}

int pwm_set_duty(unsigned int channel, unsigned int duty) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  if (duty > PWM_MAX_PERIOD) {
    return PWM_ERROR;
  }
  PWM_DUTY(channel) = duty & PWM_16BIT_MASK;
  return PWM_OK;
}

int pwm_enable(unsigned int channel) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  PWM_CTRL(channel) |= PWM_CTRL_ENABLE;
  return PWM_OK;
}

int pwm_disable(unsigned int channel) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  PWM_CTRL(channel) &= ~PWM_CTRL_ENABLE;
  return PWM_OK;
}

int pwm_start(unsigned int channel, unsigned int period, unsigned int duty) {
  if (pwm_check_channel(channel) != PWM_OK) {
    return PWM_ERROR;
  }
  if (period > PWM_MAX_PERIOD || duty > PWM_MAX_PERIOD) {
    return PWM_ERROR;
  }
  PWM_PERIOD(channel) = period & PWM_16BIT_MASK;
  PWM_DUTY(channel) = duty & PWM_16BIT_MASK;
  PWM_CTRL(channel) |= PWM_CTRL_ENABLE;
  return PWM_OK;
}

unsigned int pwm_get_counter(unsigned int channel) {
  if (channel > PWM_CHANNEL_MAX) {
    return 0;
  }
  return PWM_COUNTER(channel) & PWM_16BIT_MASK;
}
