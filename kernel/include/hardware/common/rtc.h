#pragma once
#include <stdint.h>
#include <time.h>
#include <hardware/interrupts/idt.h>

const double RTC_MILLIS_TIME = 0.9765625;

#define RTC_PORT_CMD  0x70
#define RTC_PORT_DATA 0x71

namespace os
{
    namespace hal
    {
        class rtc
        {
            private:
                static std::time_t    _tm;
                static uint32_t       _ticks;
                static uint32_t       _timer;
                static uint32_t       _now, _last;
                static double         _millis;

            public:
                static void    init();
                static void    callback(idt_registers_t* regs);
                static void    fetch();

            public:
                static std::time_t* time();
            
            private:
                static uint8_t read_reg(uint16_t reg);
        };
    }
}