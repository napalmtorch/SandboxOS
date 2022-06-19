#include <hardware/devices/keyboard.h>
#include <core/kernel.h>

namespace os
{
    namespace hal
    {
        namespace devices { ps2_keyboard* kbd = NULL; }

        void ps2_keyboard::init()
        {
            this->_keymap = std::array<bool>(256);
            this->_code   = 0;
            this->_state  = NULL;
            set_info("PS/2 Keyboard", 0xD0000700);
        }

        void ps2_keyboard::start()
        {

        }

        void ps2_keyboard::stop()
        {

        }

        void ps2_keyboard::handle(uint8_t data)
        {
            _code = data;
            update_keymap();
            update_state();
        }

        void ps2_keyboard::update_keymap()
        {
            if (_code <= 0x58) { _keymap[_code] = true; }
            else if (_code >= 0x81 && _code < 0xD8) { _keymap[_code - 128] = false; }
        }

        void ps2_keyboard::update_state()
        {
            if (_state == NULL) { return; }
            std::key_code key = (std::key_code)_code;

            if (key == std::key_code::backspace)
            {
                if (strlen(_state->instream.ptr()) > 0 && _state->instream.ptr() != NULL)
                {
                    strback(_state->instream.ptr());
                    if (_state->on_backspace != NULL) { _state->on_backspace(_state->host, NULL); }
                }
            }
            else if (key == std::key_code::lshift)     { _state->left_shift = true; }
            else if (key == std::key_code::rshift)     { _state->right_shift = true; }
            else if (_code == 0xAA)                     { _state->left_shift = false; }
            else if (_code == 0xB6)                      { _state->right_shift = false; }
            else if (key == std::key_code::caps_lock)  { _state->caps_lock = !_state->caps_lock; }
            else if (key == std::key_code::lctrl)      { _state->control = true; }
            else if (key == std::key_code::lalt)       { _state->alt = true; }
            else if (key == std::key_code::escape)     { _state->escape = true; }
            else if (key == std::key_code::enter)
            {
                if (_state->newline && _state->instream.ptr() != NULL) { stradd(_state->instream.ptr(), '\n'); }
                if (_state->on_enter != NULL) { _state->on_enter(_state->host, NULL); }
            }   
            else if (_code < 58)
            {
                char ascii = 0;
                bool caps = (((_state->left_shift || _state->right_shift) && !_state->caps_lock) || ((!_state->left_shift && !_state->right_shift && _state->caps_lock)));

                if (caps) { ascii = std::KB_LAYOUT_US.upper[_code]; }
                else      { ascii = std::KB_LAYOUT_US.lower[_code]; }

                if (ascii >= 32 && ascii <= 126 && _state->instream.ptr() != NULL) { stradd(_state->instream.ptr(), ascii); }
                if (_state->on_char != NULL) { _state->on_char(_state->host, (void*)ascii); }
            }
        }

        void ps2_keyboard::attach_state(std::keyboard_state_t* state) { this->_state = state; }
    
        std::keyboard_state_t* ps2_keyboard::get_state() { return this->_state; }

        bool ps2_keyboard::is_keydown(std::key_code key) { return this->_keymap[(int)key]; }

        bool ps2_keyboard::is_keyup(std::key_code key) { return !this->_keymap[(int)key]; }
    }
}