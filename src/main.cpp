
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

#include "uart.hpp"
#include "sys_clock.hpp"
#include "random.hpp"

USART<1> uart(&SystemCoreClock);
RANDOM<2023> Random;
uint8_t global_array[256];

extern "C" void SysTick_Handler(void)
{
  SysTickHandler();
}

extern "C" void USART1_IRQHandler(void)
{
  uart.IRQHandler();
}


int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  uart.Init(115200, 8, NO, 1);
  Random.Init();
  while (true)
  {
    delay_ms(Random.get(500, 2500));

    uint8_t size = Random.getByte();
    for (uint8_t i = 0; i < size; i++)
      global_array[i] = Random.getByte('a', 'z');
    uart.TX(global_array, size);

    const uint8_t const_line[] = "0123456789";
    uart.TX(const_cast<uint8_t *>(const_line), sizeof(const_line) - 1);

    uint8_t end_line[] = "\r\n"; // {13, 10, 0}
    uart.TX(end_line, sizeof end_line);
  }
}
