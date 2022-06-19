#include <hardware/devices/ps2.h>
#include <core/kernel.h>

#define PS2_DATA           0x60
#define PS2_STATUS         0x64
#define PS2_COMMAND        0x64
#define MOUSE_WRITE        0xD4
#define MOUSE_V_BIT        0x08

#define PS2_PORT1_IRQ      0x01
#define PS2_PORT2_IRQ      0x02
#define PS2_PORT1_TLATE    0x40

#define PS2_READ_CONFIG    0x20
#define PS2_WRITE_CONFIG   0x60

#define PS2_DISABLE_PORT2  0xA7
#define PS2_ENABLE_PORT2   0xA8
#define PS2_DISABLE_PORT1  0xAD
#define PS2_ENABLE_PORT1   0xAE

#define MOUSE_SET_REMOTE   0xF0
#define MOUSE_DEVICE_ID    0xF2
#define MOUSE_SAMPLE_RATE  0xF3
#define MOUSE_DATA_ON      0xF4
#define MOUSE_DATA_OFF     0xF5
#define MOUSE_SET_DEFAULTS 0xF6

#define MOUSE_DEFAULT         0
#define MOUSE_SCROLLWHEEL     1
#define MOUSE_BUTTONS         2

#define KBD_SET_SCANCODE   0xF0

namespace os
{
    namespace hal
    {
        void ps2_controller::init()
        {
            asm volatile("cli");

            send_cmd(PS2_DISABLE_PORT1);
            send_cmd(PS2_DISABLE_PORT2);
            while ((ports::inb(PS2_STATUS) & 1)) { ports::inb(PS2_DATA); }            

            uint8_t status = rs_cmd(PS2_READ_CONFIG);
            status |= (PS2_PORT1_IRQ | PS2_PORT2_IRQ | PS2_PORT1_TLATE);
            send_cmd(PS2_WRITE_CONFIG, status);
            printf("%s Sent config byte to PS/2 controller\n", DEBUG_INFO);

            send_cmd(PS2_ENABLE_PORT1);
            send_cmd(PS2_ENABLE_PORT2);
            printf("%s Enabled PS/2 ports\n", DEBUG_INFO);

            idt::register_irq(IRQ::IRQ1, callback);
            idt::register_irq(IRQ::IRQ12, callback);
            printf("%s Registered PS/2 interrupts\n", DEBUG_INFO);

            write_kb(KBD_SET_SCANCODE);
            write_kb(2);
            printf("%s Set keyboard scancode\n", DEBUG_INFO);

            write_ms(MOUSE_SET_DEFAULTS);
            write_ms(MOUSE_DATA_ON);
            printf("%s Set default mouse settings\n", DEBUG_INFO);

            asm volatile("sti");
            printf("%s Initialized PS/2 controller\n", DEBUG_OK);
        }

        int ps2_controller::wait_input()
        {
            uint32_t timeout = 100000;
            while (--timeout) { if (!(ports::inb(PS2_STATUS) & (1 << 1))) { return 0; } }
            return 1;
        }

        int ps2_controller::wait_output()
        {
            uint32_t timeout = 100000;
            while (--timeout) { if (ports::inb(PS2_STATUS) & (1 << 0)) { return 0; } }
            return 1;
        }

        void ps2_controller::set_samplerate(uint8_t rate)
        {
            write_ms(MOUSE_SAMPLE_RATE);
            while (!(ports::inb(PS2_STATUS) & 1));
            ports::inb(PS2_DATA);
            write_ms(rate);
            while (!(ports::inb(PS2_STATUS) & 1));
            ports::inb(PS2_DATA);
        }

        void ps2_controller::send_cmd(uint8_t cmd)
        {
            wait_input();
            ports::outb(PS2_COMMAND, cmd);
        }

        void ps2_controller::send_cmd(uint8_t cmd, uint8_t arg)
        {
            wait_input();
            ports::outb(PS2_COMMAND, cmd);
            wait_input();
            ports::outb(PS2_DATA, arg);
        }

        uint8_t ps2_controller::rs_cmd(uint8_t cmd)
        {
            wait_input();
            ports::outb(PS2_COMMAND, cmd);
            wait_output();
            return ports::inb(PS2_DATA);
        }

        void ps2_controller::write_ms(uint8_t data)
        {
            send_cmd(MOUSE_WRITE, data);
        }
    
        void ps2_controller::write_kb(uint8_t data)
        {
            wait_input();
            ports::outb(PS2_DATA, data);
        }

        void ps2_controller::callback(idt_registers_t* regs)
        {
            send_cmd(PS2_DISABLE_PORT1);
            send_cmd(PS2_DISABLE_PORT2);

            uint8_t status = ports::inb(PS2_STATUS);
            uint8_t data   = ports::inb(PS2_DATA);

            send_cmd(PS2_ENABLE_PORT1);
            send_cmd(PS2_ENABLE_PORT2);

            idt::ack_irq(regs);

            if (!(status & 0x01))   { return; }
            if (!(status & 0x20))   { devices::kbd->handle(data); return; }
            if (status & 0x21)      { devices::mouse->handle(data); }
        }

        void ps2_controller::callback_ms(idt_registers_t* regs)
        {

        }

        void ps2_controller::callback_kb(idt_registers_t* regs)
        {

        }
    }
}