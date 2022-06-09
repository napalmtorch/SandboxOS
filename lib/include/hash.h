#pragma once
#include <stdint.h>
#include <math.h>
#include <string.h>

namespace std
{
    static inline uint32_t hash(char* str)
    {
        if (str == NULL || strlen(str) == 0) { return 0; }

        uint32_t h = 5381;
        int c = 0;

        while (c = *str++) { h = ((h << 5) + h) + c; }
        return h;
    }
}