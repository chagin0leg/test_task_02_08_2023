#include "sys_clock.h"

void delay_ms(SYS_TICK *sys_tick, uint32_t ms)
{
  sys_tick->SysTick_CNT = ms;
  while (sys_tick->SysTick_CNT)
    ;
}

void SysTickHandler(SYS_TICK *sys_tick)
{
  if (sys_tick->SysTick_CNT > 0)
    sys_tick->SysTick_CNT--;
}
