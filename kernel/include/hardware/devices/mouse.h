#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>
#include <hardware/device.h>

namespace os
{
    namespace hal
    {
        typedef struct
        {
            bool btn_left   : 1;
            bool btn_right  : 1;
            bool btn_middle : 1;
            bool reserved   : 1;
            bool sign_x     : 1;
            bool sign_y     : 1;
            bool overflow_x : 1;
            bool overflow_y : 1;
        } PACKED mspacket_bits_t;

        typedef union
        {
            uint8_t         value;
            mspacket_bits_t bits;
        } mspacket_flags_t;

        typedef struct
        {
            int8_t           delta_x;
            int8_t           delta_y;
            mspacket_flags_t flags;
        } PACKED mspacket_t;

        class ps2_mouse : public device_t
        {
            private:
                mspacket_t _packet;
                int        _x, _y, _cycle;
                bool       _handled;

            public:
                void init() override;
                void start() override;
                void stop() override;
                void handle(uint8_t data);
                void move(int8_t x, int8_t y);

            private:
                bool check_overflow();

            public:
                std::ivec2d_t position();
                bool btn_left();
                bool btn_right();
                bool btn_middle();
        };

        namespace devices
        {
            extern ps2_mouse* mouse;
        }
    }
}