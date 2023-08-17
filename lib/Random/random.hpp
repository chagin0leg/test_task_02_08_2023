#ifndef __RANDOM_HPP
#define __RANDOM_HPP

#include <stdint.h>
#include <stdlib.h>

template <uint32_t seed>
class RANDOM
{
public:
    RANDOM(){

    };

    void Init(void)
    {
        srand(seed);
    }

    uint8_t getByte(uint8_t min = 0, uint8_t max = 0xFF)
    {
        return uint8_t(((rand() % (max - min + 1)) + min)& 0xFF);
    }

    int16_t get(int16_t min = 0, int16_t max = 0x7FFF)
    {
        return int16_t((rand() % (max - min + 1)) + min);
    }
};

#endif // !__RANDOM_HPP
