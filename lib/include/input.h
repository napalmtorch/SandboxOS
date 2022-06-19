#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stream.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    enum class key_code : uint8_t
    {
        invalid,
        escape,
        d1,
        d2,
        d3,
        d4,
        d5,
        d6,
        d7,
        d8,
        d9,
        d0,
        minus,
        equal,
        backspace,
        tab,
        q,
        w,
        e,
        r,
        t,
        y,
        u,
        i,
        o,
        p,
        lbracket,
        rbracket,
        enter,
        lctrl,
        a,
        s,
        d,
        f,
        g,
        h,
        j,
        k,
        l,
        colon,
        quote,
        backtick,
        lshift,
        backslash,
        z,
        x,
        c,
        v,
        b,
        n,
        m,
        comma,
        period,
        slash,
        rshift,
        kp_multiply,
        lalt,
        space,
        caps_lock,
        f1,
        f2,
        f3,
        f4,
        f5,
        f6,
        f7,
        f8,
        f9,
        f10,
        num_lock,
        scroll_lock,
        kp_home,
        kp_up,
        kp_pgup,
        kp_minus,
        kp_left,
        kp_d5,
        kp_right,
        kp_plus,
        kp_end,
        kp_down,
        kp_pgdn,
        kp_period,
        f11 = 0x57,
        f12,
    };

    typedef void (*keyboard_event_t)(void* sender, void* arg);

    typedef struct
    {
        char upper[60];
        char lower[60];
    } keyboard_layout_t;

    typedef struct
    {
        keyboard_event_t on_char;
        keyboard_event_t on_enter;
        keyboard_event_t on_backspace;
        stream<char>     instream;
        void*            host;

        bool left_shift  : 1;
        bool right_shift : 1;
        bool control     : 1;
        bool alt         : 1;
        bool caps_lock   : 1;
        bool escape      : 1;
        bool newline     : 1;
    } PACKED keyboard_state_t;

    static const keyboard_layout_t KB_LAYOUT_US = (keyboard_layout_t)
    {
        .upper = "??!@#$%^&*()_+??QWERTYUIOP{}??ASDFGHJKL:\"~?|ZXCVBNM<>???? \0",
        .lower = "??1234567890-=??qwertyuiop[]??asdfghjkl;'`?\\zxcvbnm,./??? \0",
    };

    enum class mouse_btn : uint8_t
    {
        left,
        right,
        middle,
    };

    bool is_keydown(key_code key);
    bool is_keyup(key_code key);

    bool is_msdown(mouse_btn btn);
    bool is_msup(mouse_btn btn);

    ivec2d_t mspos();
}