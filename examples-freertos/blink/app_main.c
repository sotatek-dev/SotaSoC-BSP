/*
 * Blink application: uses GPIO driver in blinkTask.
 */
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "gpio_driver.h"

#define BLINK_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 2 )
#define BLINK_PIN  0
#define BLINK_DELAY_MS  500

static void blinkTask( void * param );

/*-----------------------------------------------------------*/

void vApplicationStart(void) {
  xTaskCreate(blinkTask, "Blink", BLINK_TASK_STACK_SIZE, NULL, 8, NULL);

  printf("Starting tasks...\n");
  vTaskStartScheduler();

  for (;;) printf("Scheduler ended unexpectedly.\n");
}

/*-----------------------------------------------------------*/

static void blinkTask( void * param )
{
  ( void ) param;

  if (gpio_set_direction(BLINK_PIN, GPIO_OUTPUT) != GPIO_OK) {
    printf("GPIO init failed\n");
    vTaskDelete(NULL);
    return;
  }

  for (;;) {
    gpio_toggle_pin(BLINK_PIN);
    vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
  }
}
