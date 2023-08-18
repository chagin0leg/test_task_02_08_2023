
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx.h"

#include "..\lib\UART\uart.h"
#include "..\lib\SysClock\sys_clock.h"
#include "..\lib\Random\random.h"

USART uart;
SYS_TICK sys_tick = {0};

uint8_t global_array[256] = {0};

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

  const uint8_t const_line[10] = "0123456789";
  uint8_t end_line[3] = "\r\n"; // {13, 10, 0}
  uint8_t size = 0, i = 0;
  SystemInit();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000);

  USART_init(&uart, &SystemCoreClock, USART1);
  USART_begin(&uart, 115200, 8, NO, 1);
  RANDOM_init(2023);

  while (1)
  {
    delay_ms(&sys_tick, RANDOM_get(500, 2500));

    size = RANDOM_getByte(0x00, 0xFF);
    for (i = 0; i < size; i++)
      global_array[i] = RANDOM_getByte('a', 'z');
    USART_write(&uart, global_array, size);
    USART_write(&uart, (uint8_t *)const_line, sizeof(const_line) - 1);
    USART_write(&uart, end_line, sizeof end_line);
  }
}
