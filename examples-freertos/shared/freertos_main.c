/*
 * Common FreeRTOS entry and hooks. Project must implement vApplicationStart().
 */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

extern void freertos_risc_v_trap_handler( void );
extern void vApplicationStart( void );

/*-----------------------------------------------------------*/

void main( void )
{
    __asm__ volatile ( "csrw mtvec, %0" : : "r" ( freertos_risc_v_trap_handler ) );
    vApplicationStart();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    printf( "\r\n\r\nMalloc failed\r\n" );
    portDISABLE_INTERRUPTS();
    for( ; ; ) { }
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;
    printf( "\r\n\r\nStack overflow in %s\r\n", pcTaskName );
    portDISABLE_INTERRUPTS();
    for( ; ; ) { }
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char * pcFileName, uint32_t ulLine )
{
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
    printf( "ASSERT! Line %d, file %s\r\n", ( int ) ulLine, pcFileName );
    taskENTER_CRITICAL();
    {
        while( ulSetToNonZeroInDebuggerToContinue == 0 )
        {
            __asm volatile ( "NOP" );
            __asm volatile ( "NOP" );
        }
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    StackType_t * pulIdleTaskStackSize )
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/*-----------------------------------------------------------*/

int __write( int iFile, char * pcString, int iStringLength )
{
    ( void ) iFile;
    ( void ) pcString;
    return iStringLength;
}
/*-----------------------------------------------------------*/

void * malloc( size_t size )
{
    ( void ) size;
    portDISABLE_INTERRUPTS();
    for( ; ; ) { }
}
