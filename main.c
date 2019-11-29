/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>

#define LED_PORT_CLK    RST_CLK_PCLK_PORTC
#define LED_PORT        MDR_PORTC
#define LED_PORT_PIN    PORT_Pin_0


void vLed1(void *argument)
{
    while(1)
    {
        PORT_SetBits(LED_PORT, LED_PORT_PIN);
        vTaskDelay(1000);
        PORT_ResetBits(LED_PORT, LED_PORT_PIN);
        vTaskDelay(1000);
    }
}

int main()
{
    PORT_InitTypeDef GPIOInitStruct;

    RST_CLK_PCLKcmd (LED_PORT_CLK, ENABLE);

    PORT_StructInit(&GPIOInitStruct);

    GPIOInitStruct.PORT_Pin        = LED_PORT_PIN;
    GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
    GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
    GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;

    PORT_Init(LED_PORT, &GPIOInitStruct);

    xTaskCreate(vLed1, "1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
   
    vTaskStartScheduler();
}