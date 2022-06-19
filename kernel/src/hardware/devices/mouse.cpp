#include <hardware/devices/mouse.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        namespace devices { ps2_mouse* mouse = NULL; }

        void ps2_mouse::init()
        {
            set_info("PS/2 Mouse", 0xD0000701);
        }

        void ps2_mouse::start()
        {

        }

        void ps2_mouse::stop()
        {

        }

        void ps2_mouse::handle(uint8_t data)
        {
            _handled = false;

            switch (_cycle)
            {
                case 0:
                {
                    _packet.flags.value = data;
                    if (!(data & 0x08)) { return; }
                    _cycle++;
                    break;
                }
                case 1: { _packet.delta_x = data; _cycle++; break; }
                case 2:
                {
                    if (check_overflow()) { _cycle = 0; return; }
                    _packet.delta_y = (int8_t)-data;
                    _handled        = true;
                    _cycle          = 0;
                    break;
                }
            }

            if (_handled) { move(_packet.delta_x, _packet.delta_y); }
        }

        void ps2_mouse::move(int8_t x, int8_t y)
        {
            _x += x;
            _y += y;
            if (_x < 0) { _x = 0; }
            if (_y < 0) { _y = 0; }
            if (_x >= devices::vbe->modeinfo().width - 1)  { _x = devices::vbe->modeinfo().width  - 1; }
            if (_y >= devices::vbe->modeinfo().height - 1) { _y = devices::vbe->modeinfo().height - 1; }
        }

        bool ps2_mouse::check_overflow() { return (_packet.flags.bits.overflow_x || _packet.flags.bits.overflow_y); }
        
        std::ivec2d_t ps2_mouse::position() { return std::ivec2d_t(_x, _y); }

        bool ps2_mouse::btn_left() { return _packet.flags.bits.btn_left; }

        bool ps2_mouse::btn_right() { return _packet.flags.bits.btn_right; }

        bool ps2_mouse::btn_middle() { return _packet.flags.bits.btn_middle; }
    }
}