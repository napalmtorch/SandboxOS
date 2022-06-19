#pragma once
#include <stdint.h>

/// @brief Align 32-bit number to specified value @param num Input number @param align Alignment value @return Aligned value
static inline uint32_t memalign(uint32_t num, uint32_t align)
{
    uint32_t out = num;
    out &= (0xFFFFFFFF - (align - 1));
    if (out < num) { out += align; }
    return out;
}

/// @brief Lock current thread to prevent context switching
void lock();

/// @brief Unlock current thread
void unlock();

/// @brief Send request to switch active thread
void yield();

/// @brief Sleep for specified amount of milliseconds - currently only works before scheduling has started @param ms Amount of milliseconds
void sleep(uint32_t ms);