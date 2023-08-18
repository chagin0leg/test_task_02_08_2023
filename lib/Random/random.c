
#include <stdlib.h>
#include "random.h"

void RANDOM_init(uint32_t seed)
{
    srand(seed);
}

uint8_t RANDOM_getByte(uint8_t min, uint8_t max)
{
    return (uint8_t)(((rand() % (max - min + 1)) + min) & 0xFF);
}

int16_t RANDOM_get(int16_t min, int16_t max)
{
    return (int16_t)((rand() % (max - min + 1)) + min);
}
