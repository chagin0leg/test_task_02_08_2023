
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

#include "main.h"
#include "uart.h"
#include "sys_clock.h"

USART<1> uart(&SysClock);

extern "C" void USART1_IRQHandler(void)
{
    uart.IRQHandler();
}

int main(void)
{
    RCC_DeInit();
    RCC_Init();
    SetSysClock(25 MHz);
    SysTick_Init();

    uart.Init(115200, 8, NO, 1);
    while (1)
    {
        delay_ms(5000);
        uint8_t array[] = {0xAA, 0x09, 0xDC, 0x05, 0x01, 0x10, 0x31};
        uart.TX(array, sizeof array);
    }
    return 0;
}