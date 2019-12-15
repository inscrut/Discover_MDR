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
#define LED_1           PORT_Pin_0
#define LED_2           PORT_Pin_1


void vLed(void *argument)
{
  uint32_t _LED = (uint32_t) argument;
  
  while(1)
  {
    PORT_SetBits(LED_PORT, _LED);
    vTaskDelay(100 / portTICK_RATE_MS);
    PORT_ResetBits(LED_PORT, _LED);
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

int main()
{
    PORT_InitTypeDef GPIOInitStruct;

    RST_CLK_PCLKcmd (LED_PORT_CLK, ENABLE);

    PORT_StructInit(&GPIOInitStruct);

    GPIOInitStruct.PORT_Pin        = LED_1 | LED_2;
    GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
    GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
    GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;

    PORT_Init(LED_PORT, &GPIOInitStruct);

    xTaskCreate(vLed, "L1", configMINIMAL_STACK_SIZE, (void*) LED_1, 1, NULL);
    xTaskCreate(vLed, "L2", configMINIMAL_STACK_SIZE, (void*) LED_2, 1, NULL);
   
    vTaskStartScheduler();
}