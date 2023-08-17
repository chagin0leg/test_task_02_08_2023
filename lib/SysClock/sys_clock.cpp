
#include "stm32f4xx.h"
#include "sys_clock.hpp"

volatile uint32_t SysTick_CNT = 0;

void delay_ms(uint32_t ms)
{
  SysTick_CNT = ms;
  while (SysTick_CNT)
    ;
}

void SysTickHandler(void)
{
  if (SysTick_CNT > 0)
    SysTick_CNT--;
}
