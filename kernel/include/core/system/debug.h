#pragma once
#include <stdint.h>
#include <hardware/common/serial.h>
#include <hardware/interrupts/idt.h>
#include <core/exec/thread.h>

namespace os
{
    namespace sys
    {
        class debug
        {
            public:
                static hal::serial_controller serial;
                static bool                   stdout_enabled;

            public:
                static void init(hal::serial_port port);
                static void print_regs(hal::idt_registers_t* regs);
                static void print_regs(threading::thread_regs_t* regs);
        };
    }
}