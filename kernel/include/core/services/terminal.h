#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <input.h>
#include <gfx/primitives/vec2d.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/image.h>
#include <gui/window.h>

namespace os
{
    namespace services
    {
        class terminal_host : public std::gui::window
        {
            public:
                std::gfx::image tbuff;


            private:
                uint32_t _fg, _bg;
                uint32_t _cx, _cy;
                uint32_t _ow, _oh;
                uint32_t _xcount, _ycount;
                uint32_t _now, _last, _ctimer;
                bool     _cflash;
                std::gfx::bitfont _font;

            private:
                std::keyboard_state_t _kb;
                char* _dir;

            public:
                terminal_host(int x, int y, int x_count, int y_count, uint32_t fg, uint32_t bg, std::gfx::bitfont font);

            public:
                void dispose() override;
                void update() override;
                void draw() override;
                void render() override;

            private:
                static void on_char(terminal_host* sender, void* arg);
                static void on_backspace(terminal_host* sender, void* arg);
                static void on_enter(terminal_host* sender, void* arg);

            public:
                void print_caret();
                void newline(int count = 1);
                void backspace(int count = 1);
                void scroll(int count = 1);
                void resize(int w, int h);

            public:
                void clear();
                void clear(uint32_t color);
                void putc(int x, int y, char c, uint32_t fg, uint32_t bg);
                void putstr(int x, int y, char* str, uint32_t fg, uint32_t bg);
                
            public:
                void write(char c);
                void write(char c, uint32_t fg);
                void write(char c, uint32_t fg, uint32_t bg);
                void print(char* str);
                void print(char* str, uint32_t fg);
                void print(char* str, uint32_t fg, uint32_t bg);
                void println(char* str);
                void println(char* str, uint32_t fg);
                void println(char* str, uint32_t fg, uint32_t bg);

            public:
                void set_colors(uint32_t fg, uint32_t bg);
                void set_forecolor(uint32_t color);
                void set_backcolor(uint32_t color);
                uint32_t forecolor();
                uint32_t backcolor();

            public:
                void set_cursor(int x, int y);
                void set_cursor_x(int x);
                void set_cursor_y(int y);
                std::ivec2d_t cursor();

            public:
                void set_font(std::gfx::bitfont font);
                std::gfx::bitfont* font();
        };
    }
}