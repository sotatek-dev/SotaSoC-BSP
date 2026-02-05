/*
 * HelloWorld application: implements vApplicationStart() (create tasks, start scheduler).
 */
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#define PRINT_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 2 )

static void printTask1( void * param );
static void printTask2( void * param );

/*-----------------------------------------------------------*/

void vApplicationStart(void) {
  xTaskCreate(printTask1, "Print1", PRINT_TASK_STACK_SIZE, NULL, 8, NULL);
  xTaskCreate(printTask2, "Print2", PRINT_TASK_STACK_SIZE, NULL, 8, NULL);

  printf("Starting tasks...\n");
  vTaskStartScheduler();

  for (;;) printf("Scheduler ended unexpectedly.\n");
}

/*-----------------------------------------------------------*/

static void printTask1( void * param )
{
    volatile uint32_t r1 = 1, r2 = 2, r3 = 3, r4 = 4;
    volatile uint32_t r5 = 5, r6 = 6, r7 = 7, r8 = 8;

    ( void ) param;
    for( ; ; )
    {
        r1++; r2++; r3++; r4++;
        r5++; r6++; r7++; r8++;

        portENTER_CRITICAL();
        printf( "T1: %u %u %u %u\n", r1, r2, r3, r4 );
        portEXIT_CRITICAL();

        taskYIELD();

        if( r2 != r1 + 1 || r3 != r1 + 2 || r8 != r1 + 7 )
        {
            printf( "CRITICAL ERROR: Register Corruption in Task 1!\n" );
            for( ; ; ) { }
        }
    }
}

static void printTask2( void * param )
{
    volatile uint32_t k1 = 1000, k2 = 1001, k3 = 1002, k4 = 1003;
    volatile uint32_t k5 = 1004, k6 = 1005, k7 = 1006, k8 = 1007;

    ( void ) param;
    for( ; ; )
    {
        k1++; k2++; k3++; k4++;
        k5++; k6++; k7++; k8++;

        portENTER_CRITICAL();
        printf( "T2: %u %u %u %u\n", k1, k2, k3, k4 );
        portEXIT_CRITICAL();

        taskYIELD();

        if( k1 < 1000 || k2 != k1 + 1 || k8 != k1 + 7 )
        {
            printf( "CRITICAL ERROR: Register Corruption in Task 2!\n" );
            for( ; ; ) { }
        }
    }
}
