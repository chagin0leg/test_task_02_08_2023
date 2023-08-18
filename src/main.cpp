
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

#include "uart.h"
#include "sys_clock.h"
#include "random.h"

USART uart;
RANDOM Random = {_init, _getByte, _get};
SYS_TICK sys_tick = {0};

void SysTick_Handler(void)
{
  SysTickHandler(&sys_tick);
}

void USART1_IRQHandler(void)
{
  USART_IRQHandler(&uart);
}

int main(void)
{
  SystemInit();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  USART_init(&uart, &SystemCoreClock, USART1);
  USART_begin(&uart, 115200, 8, NO, 1);
  Random.Init(2023);

  uint8_t global_array[256] = {0};
  while (1)
  {
    delay_ms(&sys_tick, Random.get(500, 2500));

    uint8_t size = Random.getByte(0x00, 0xFF);
    for (uint8_t i = 0; i < size; i++)
      global_array[i] = Random.getByte('a', 'z');
    USART_write(&uart, global_array, size);

    const uint8_t const_line[] = "0123456789";
    USART_write(&uart,(uint8_t*)const_line, sizeof(const_line) - 1);

    uint8_t end_line[] = "\r\n"; // {13, 10, 0}
    USART_write(&uart, end_line, sizeof end_line);
  }
}
