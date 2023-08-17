#ifndef __RANDOM_H
#define __RANDOM_H

#include <stdint.h>
#include <stdlib.h>


void _init(uint32_t seed)
{
    srand(seed);
}

uint8_t _getByte(uint8_t min, uint8_t max)
{
    return (uint8_t) (((rand() % (max - min + 1)) + min) & 0xFF);
}

int16_t _get(int16_t min, int16_t max)
{
    return (int16_t) ((rand() % (max - min + 1)) + min);
}

typedef struct
{
    void (*Init)(uint32_t seed);
    uint8_t (*getByte)(uint8_t min, uint8_t max);
    int16_t (*get)(int16_t min, int16_t max);
} RANDOM;

#endif // !__RANDOM_H
