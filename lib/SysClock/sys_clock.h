#ifndef __SYS_CLOCK_H
#define __SYS_CLOCK_H

#include <stdint.h>

typedef struct SYS_TICK
{
  volatile uint32_t SysTick_CNT;
} SYS_TICK;

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


#endif // !__SYS_CLOCK_H
