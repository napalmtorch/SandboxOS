#pragma once
#include <stdint.h>
#include <stddef.h>
#include <hardware/interrupts/idt.h>

namespace os
{
    namespace hal
    {
        class pit
        {
            private:
                static uint32_t _freq;
                static uint32_t _ticks, _timer;
                static uint32_t _millis, _millis_total, _seconds;
                static uint32_t _switch_timer;

            public:
                static void init();
                static void callback(idt_registers_t* regs);
                static void update();

            public:
                static uint32_t freq();
                static uint32_t ticks();
                static uint32_t millis();
                static uint32_t millis_total();
                static uint32_t seconds();
        };
    }
}