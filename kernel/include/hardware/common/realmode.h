#pragma once
#include <stdint.h>
#include <stddef.h>

namespace os
{
    namespace hal
    {
        typedef struct
        {
            uint16_t di, si, bp, sp, bx, dx, cx, ax;
            uint16_t gs, fs, es, ds, eflags;
        } PACKED registers16_t;
    }
}

EXTC { extern void int32(uint8_t interrupt, os::hal::registers16_t* regs); }