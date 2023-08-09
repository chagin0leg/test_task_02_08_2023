#ifndef __SYS_CLOCK_H
#define __SYS_CLOCK_H

#include <stdint.h>

#define GHz *1000000000
#define MHz *1000000
#define kHz *1000
#define Hz  *1

void RCC_DeInit(void);
void RCC_Init(void);
void SetSysClock(uint32_t sys_clock);
void SysTick_Init(void);
void delay_ms(uint32_t ms);
void delay(volatile uint64_t tck);


extern uint32_t SysClock;

extern volatile uint32_t SysTick_CNT;


#endif // !__SYS_CLOCK_H
