#pragma once
#include <stdint.h>

EXTC
{
    static inline uint32_t memalign(uint32_t num, uint32_t align)
    {
        uint32_t out = num;
        out &= (0xFFFFFFFF - (align - 1));
        if (out < num) { out += align; }
        return out;
    }
}

void lock();
void unlock();
void yield();