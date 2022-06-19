#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <array.h>
#include <input.h>
#include <hardware/device.h>

namespace os
{
    namespace hal
    {
        class ps2_keyboard : public device_t
        {
            private:
                std::keyboard_state_t* _state;
                std::array<bool>       _keymap;
                uint8_t                 _code;

            public:
                void init() override;
                void start() override;
                void stop() override;
                void handle(uint8_t data);
                void update_keymap();
                void update_state();
                void attach_state(std::keyboard_state_t* state);

            public: 
                std::keyboard_state_t* get_state();

            public:
                bool is_keydown(std::key_code key);
                bool is_keyup(std::key_code key);
        };

        namespace devices
        {
            extern ps2_keyboard* kbd;
        }
    }
}