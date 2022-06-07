#pragma once
#include <stdint.h>
#include <stdio.h>
#include <ubsan.h>
#include <hardware/common/ports.h>

namespace os
{
    namespace hal
    {
        namespace pic
        {
            static inline void remap()
            {
                ports::outb(0x20, 0x11);
                ports::outb(0xA0, 0x11);
                ports::outb(0x21, 0x20);
                ports::outb(0xA1, 0x28);
                ports::outb(0x21, 0x04);
                ports::outb(0xA1, 0x02);
                ports::outb(0x21, 0x01);
                ports::outb(0xA1, 0x01);
                ports::outb(0x21, 0x00);
                ports::outb(0xA1, 0x00);
                printf("%s Re-mapped PIC\n", DEBUG_INFO);
            }
        }
    }
}