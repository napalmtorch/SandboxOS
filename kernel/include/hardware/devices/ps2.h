#pragma once
#include <stdint.h>
#include <hardware/interrupts/idt.h>

namespace os
{
    namespace hal
    {
        class ps2_controller
        {
            public:
                static void    init();
                static int     wait_input();
                static int     wait_output();
                static void    set_samplerate(uint8_t rate);
                static void    send_cmd(uint8_t cmd);
                static void    send_cmd(uint8_t cmd, uint8_t arg);
                static uint8_t rs_cmd(uint8_t cmd);
                static void    write_ms(uint8_t data);
                static void    write_kb(uint8_t data);
                static void    callback(idt_registers_t* regs);
                static void    callback_ms(idt_registers_t* regs);
                static void    callback_kb(idt_registers_t* regs);
        };
    }
}