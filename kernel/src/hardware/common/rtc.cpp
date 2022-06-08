#include <hardware/common/rtc.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        std::time_t    rtc::_tm;
        uint32_t       rtc::_ticks;
        uint32_t       rtc::_timer;
        uint32_t       rtc::_now;
        uint32_t       rtc::_last;
        double         rtc::_millis;

        void rtc::init()
        {
            _ticks = 0;
            _timer = 0;
            _now = 0;
            _last = 0;
            _millis = 0;
            memset(&_tm, 0, sizeof(std::time_t));

            asm volatile("cli");
            ports::outb(RTC_PORT_CMD, 0x8B);	
            char prev = ports::inb(RTC_PORT_DATA);
            ports::outb(RTC_PORT_CMD, 0x8B);
            ports::outb(RTC_PORT_DATA, prev | 0x40);
            ports::outb(RTC_PORT_CMD, 0x0C);
            (void)ports::inb(RTC_PORT_DATA);

            idt::register_irq(IRQ::IRQ8, callback);

            asm volatile("sti");
            printf("%s Initialized RTC controller\n", DEBUG_OK);
        }

        void rtc::callback(idt_registers_t* regs)
        {
            idt::ack_irq(regs);

            _ticks++;
            _timer++;
            _millis++;
            _now = _tm.second;
            if (_now != _last) { _last = _now; _millis = 0; }
            if (_timer >= 512) { _timer = 0; fetch(); }

            ports::outb(RTC_PORT_CMD, 0x0C);
            (void)ports::inb(RTC_PORT_DATA);
        }
        
        void rtc::fetch()
        {
            // read data from registers
            _tm.second = read_reg(0x00);
            _tm.minute = read_reg(0x02);
            _tm.hour   = read_reg(0x04);
            _tm.day    = read_reg(0x07);
            _tm.month  = read_reg(0x08);
            _tm.year   = read_reg(0x09);

            // check if data is bcd
            uint8_t bcd = read_reg(0x0B);

            // decode bcd formatted data
            if (!(bcd & 0x04))
            {
                _tm.second  = (_tm.second & 0x0F) + (_tm.second / 16) * 10;
                _tm.minute  = (_tm.minute & 0x0F) + (_tm.minute / 16) * 10;
                _tm.hour    = ((_tm.hour & 0x0F)  + (_tm.hour / 16) * 10) | (_tm.hour & 0x80);
                _tm.day     = (_tm.day & 0x0F)    + (_tm.day / 16) * 10;
                _tm.month   = (_tm.month & 0x0F)  + (_tm.month / 16) * 10;
                _tm.year    = (_tm.year & 0x0F)   + (_tm.year / 16) * 10;
                _tm.year    += 2000;
            }
        }
        
        uint8_t rtc::read_reg(uint16_t reg)
        {
            ports::outb(RTC_PORT_CMD, (uint8_t)reg);
            return ports::inb(RTC_PORT_DATA);
        }

        std::time_t* rtc::time() { return &_tm; }

    }
}