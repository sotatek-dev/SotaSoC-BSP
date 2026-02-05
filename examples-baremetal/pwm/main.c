#include "pwm_driver.h"

// PWM example: channel 0 at 1 kHz with duty cycle ramping (breathing LED effect)
// Assumes PWM output is connected to an LED or scope.

#define PWM_FREQ_HZ     1000
#define PWM_PERIOD_TICKS  (CLK_HZ / PWM_FREQ_HZ)
#define DUTY_STEP       1024
#define DELAY_LOOPS     4000

int main(void) {
  pwm_init(PWM_CHANNEL_0);
  pwm_set_period(PWM_CHANNEL_0, PWM_PERIOD_TICKS);
  pwm_enable(PWM_CHANNEL_0);

  unsigned int duty = 0;
  int step = (int)DUTY_STEP;

  while (1) {
    pwm_set_duty(PWM_CHANNEL_0, duty);

    for (volatile int i = 0; i < DELAY_LOOPS; i++) {
    }

    if (step > 0) {
      duty += (unsigned int)step;
      if (duty >= PWM_PERIOD_TICKS) {
        duty = PWM_PERIOD_TICKS;
        step = -(int)DUTY_STEP;
      }
    } else {
      if (duty >= (unsigned int)DUTY_STEP) {
        duty -= (unsigned int)DUTY_STEP;
      } else {
        duty = 0;
      }
      if (duty == 0) {
        step = (int)DUTY_STEP;
      }
    }
  }
}
