
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

#include "main.hpp"
#include "uart.hpp"
#include "sys_clock.hpp"

USART<1> uart(&SystemCoreClock);

extern "C" void USART1_IRQHandler(void)
{
    uart.IRQHandler();
}

extern "C" void SysTick_Handler(void)
{
  if (SysTick_CNT > 0)
    SysTick_CNT--;
}

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock / 1000);

		delay_ms(1000);
    uart.Init(115200, 8, NO, 1);
    while (true)
    {
				delay_ms(1000);
        uint8_t array[] = {0xAA, 0x09, 0xDC, 0x05, 0x01, 0x10, 0x31};
        uart.TX(array, sizeof array);
    }
}
