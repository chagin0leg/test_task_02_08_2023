#ifndef __RANDOM_H
#define __RANDOM_H

#include <stdint.h>

void RANDOM_init(uint32_t seed);

uint8_t RANDOM_getByte(uint8_t min, uint8_t max);

int16_t RANDOM_get(int16_t min, int16_t max);

#endif // !__RANDOM_H
