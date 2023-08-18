#ifndef __SYS_CLOCK_H
#define __SYS_CLOCK_H

#include <stdint.h>

typedef struct SYS_TICK
{
  volatile uint32_t SysTick_CNT;
} SYS_TICK;

void delay_ms(SYS_TICK *sys_tick, uint32_t ms);

void SysTickHandler(SYS_TICK *sys_tick);

#endif // !__SYS_CLOCK_H
