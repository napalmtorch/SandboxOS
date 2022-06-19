#pragma once
#include <stdint.h>
#include <gfx/image.h>
#include <hardware/common/serial.h>
#include <hardware/interrupts/idt.h>
#include <core/exec/thread.h>

namespace os
{
    namespace sys
    {
        class debug
        {
            private:
                static hal::serial_controller _dbg_serial;

            public:
                /// @brief Serial controller for debugging output
                static hal::serial_controller* serial;
                /// @brief Allow printf to output to serial
                static bool stdout_enabled;

            public:
                /// @brief Initialize debugging interface @param port Serial port
                static void init(hal::serial_port port);
                /// @brief Disable debugging output
                static void disable();
                /// @brief Print IDT register values @param regs Pointer to register structure
                static void print_regs(hal::idt_registers_t* regs);
                /// @brief Print thread register values @param regs Pointer to register structure
                static void print_regs(threading::thread_regs_t* regs);

            public:
                /// @brief Draw VBE graphical overlay containing debugging information
                static void draw_overlay(std::gfx::image* img);
        };
    }
}