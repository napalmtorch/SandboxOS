#include <hardware/common/pit.h>
#include <core/kernel.h>

#define PIT_CMD  0x43
#define PIT_DATA 0x40

namespace os
{
    namespace hal
    {
        uint32_t pit::_freq;
        uint32_t pit::_ticks, pit::_timer;
        uint32_t pit::_millis, pit::_millis_total, pit::_seconds;
        uint32_t pit::_switch_timer;

        void pit::init()
        {
            _freq         = 5000;
            _ticks        = 0;
            _timer        = 0;
            _millis       = 0;
            _millis_total = 0;
            _seconds      = 0;
            _switch_timer = 0;

            uint32_t f = 1193180 / _freq;
            uint8_t high = (uint8_t)((f & 0xFF00) >> 8);
            uint8_t low  = (uint8_t)((f & 0x00FF));

            ports::outb(PIT_CMD, 0x36);
            ports::outb(PIT_DATA, low);
            ports::outb(PIT_DATA, high);

            idt::register_irq(IRQ::IRQ0, callback);
            printf("%s Initialized PIT at %uHz\n", DEBUG_OK, _freq);
        }

        void pit::callback(idt_registers_t* regs)
        {
            idt::ack_irq(regs);
            update();
            threading::scheduler::yield();
        }

        void pit::update()
        {
            _ticks++;
            _timer++;

            if (_timer >= _freq / 1000)
            {
                _millis++;
                _millis_total++;
                _timer = 0;
            }

            if (_millis >= 1000)
            {
                _seconds++;
                _millis = 0;
            }
        }

        uint32_t pit::freq() { return _freq; }

        uint32_t pit::ticks() { return _ticks; }

        uint32_t pit::millis() { return _millis; }

        uint32_t pit::millis_total() { return _millis_total; }

        uint32_t pit::seconds() { return _seconds; }
    }
}