#include <input.h>
#include <core/kernel.h>

namespace std
{
    bool is_keydown(key_code key) { return os::hal::devices::kbd->is_keydown(key); }
    bool is_keyup(key_code key)   { return os::hal::devices::kbd->is_keyup(key); }

    bool is_msdown(mouse_btn btn)
    {
        switch (btn)
        {
            default:                { return false; }
            case mouse_btn::left:   { return os::hal::devices::mouse->btn_left(); }
            case mouse_btn::right:  { return os::hal::devices::mouse->btn_right(); }
            case mouse_btn::middle: { return os::hal::devices::mouse->btn_middle(); }
        }
    }

    bool is_msup(mouse_btn btn)
    {
        switch (btn)
        {
            default:                { return false; }
            case mouse_btn::left:   { return !os::hal::devices::mouse->btn_left(); }
            case mouse_btn::right:  { return !os::hal::devices::mouse->btn_right(); }
            case mouse_btn::middle: { return !os::hal::devices::mouse->btn_middle(); }
        }
    }

    ivec2d_t mspos() { return os::hal::devices::mouse->position(); }
}