#pragma once
#include <stdint.h>
#include <stddef.h>
#include <hardware/interrupts/idt.h>

namespace os
{
    namespace hal
    {
        /// @brief Programmable interval timer controller
        class pit
        {
            private:
                /// @brief Frequency of timer in hertz
                static uint32_t _freq;
                /// @brief Total amount of ticks
                static uint32_t _ticks;
                /// @brief Update timer
                static uint32_t _timer;
                /// @brief Amount of milliseconds
                static uint32_t _millis;
                /// @brief Total amount of milliseconds
                static uint32_t _millis_total;
                /// @brief Total amount of seconds
                static uint32_t _seconds;

            public:
                /// @brief Initialize programmable interval timer
                static void init();
                /// @brief Interrupt callback method @param regs Pointer to interrupt registers
                static void callback(idt_registers_t* regs);
                /// @brief Update timer and values
                static void update();

            public:
                /// @brief Get frequency in hertz
                static uint32_t freq();
                /// @brief Get total amount of ticks
                static uint32_t ticks();
                /// @brief Get amount of milliseconds
                static uint32_t millis();
                /// @brief Get total amount of milliseconds
                static uint32_t millis_total();
                /// @brief Get total amount of seconds
                static uint32_t seconds();
        };
    }
}