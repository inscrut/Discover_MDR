/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include <MDR32F9Qx_dac.h>

#define LED_PORT_CLK    RST_CLK_PCLK_PORTC
#define LED_PORT        MDR_PORTC
#define LED_1           PORT_Pin_0
#define LED_2           PORT_Pin_1

#define DAC_PORT_CLK    RST_CLK_PCLK_DAC
#define DAC_PORT        MDR_PORTE
#define DAC_PIN         PORT_Pin_0

void vLed(void *argument)
{
  uint32_t _LED = (uint32_t) argument;
  
  while(1)
  {
    PORT_SetBits(LED_PORT, _LED);
    vTaskDelay(1000 / portTICK_RATE_MS);
    PORT_ResetBits(LED_PORT, _LED);
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}

void vDAC(void *argument){
  DAC2_Cmd(ENABLE);
  xTaskCreate(vLed, "L2", configMINIMAL_STACK_SIZE, (void*) LED_2, 1, NULL);
  
  while(1){
    for(uint32_t i = 0; i < 500; i+=10){
      DAC2_SetData(i);
      vTaskDelay(1);
    }
    for(uint32_t i = 500; i > 0; i-=10){
      DAC2_SetData(i);
      vTaskDelay(1);
    }
  }
}

int main()
{
    SystemInit();
    
    PORT_InitTypeDef GPIOInitStruct;
    
    //Init LED
    RST_CLK_PCLKcmd (LED_PORT_CLK, ENABLE);
    PORT_StructInit(&GPIOInitStruct);
    GPIOInitStruct.PORT_Pin        = LED_1 | LED_2;
    GPIOInitStruct.PORT_OE         = PORT_OE_OUT;
    GPIOInitStruct.PORT_SPEED      = PORT_SPEED_SLOW;
    GPIOInitStruct.PORT_MODE       = PORT_MODE_DIGITAL;
    PORT_Init(LED_PORT, &GPIOInitStruct);
    
    //init DAC
    RST_CLK_PCLKcmd (DAC_PORT_CLK, ENABLE);
    PORT_StructInit(&GPIOInitStruct);
    GPIOInitStruct.PORT_Pin = DAC_PIN;
    GPIOInitStruct.PORT_OE = PORT_OE_OUT;
    GPIOInitStruct.PORT_MODE = PORT_MODE_ANALOG;
    PORT_Init(DAC_PORT, &GPIOInitStruct);
    DAC2_Init(DAC2_AVCC); //ref
    //DAC2_Cmd(ENABLE);
    
    //init tasts
    xTaskCreate(vLed, "L1", configMINIMAL_STACK_SIZE, (void*) LED_1, 2, NULL);
    xTaskCreate(vDAC, "DAC", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    
    //Start OS
    vTaskStartScheduler();
}