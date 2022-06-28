#include <core/services/terminal.h>
#include <core/kernel.h>

namespace os
{
    namespace services
    {
        terminal_host::terminal_host(int x, int y, int x_count, int y_count, uint32_t fg, uint32_t bg, std::gfx::bitfont font) : std::gui::window(x, y, (x_count * font.width()) + 2, (y_count * font.height()) + 24, "Terminal")
        {
            _info.style = new std::gui::visual_style();
            memcpy(_info.style, std::gui::default_styles::WINDOW, sizeof(std::gui::visual_style));
            _info.style->set_color(std::gui::color_index::bg, bg);

            _bg     = bg;
            _fg     = fg;
            _cx     = 0;
            _cy     = 1;
            _font   = font;
            _xcount = x_count;
            _ycount = y_count;
            _dir    = (char*)tmalloc(512, ALLOCTYPE_STRING);
            strcat(_dir, "A:/");

            _kb.host         = this;
            _kb.newline      = false;
            _kb.instream     = std::stream<char>(512 * KB);
            _kb.on_char      = (std::keyboard_event_t)on_char;
            _kb.on_backspace = (std::keyboard_event_t)on_backspace;
            _kb.on_enter     = (std::keyboard_event_t)on_enter;           

            tbuff = std::gfx::image(x_count * font.width(), y_count * font.height());
            _ow = tbuff.size().x;
            _oh = tbuff.size().y;

            clear();
            println("SandboxOS Terminal Client");
            println("All rights reserved.", (uint32_t)std::color32::gray);
            print_caret();
            
            update();
            draw();
            render();
        }
        
        void terminal_host::dispose()
        {
            window::dispose();
            if (hal::devices::kbd->get_state() == &_kb) { hal::devices::kbd->attach_state(NULL); }
            _kb.instream.dispose();
            tbuff.dispose();
        }

        void terminal_host::update()
        {
            window::update();

            _now = std::timenow().millis;
            if (_now != _last) { _last = _now; _ctimer++; }
            if (_ctimer >= 30) { _ctimer = 0; _cflash = !_cflash; }

            if (_flags.active && hal::devices::kbd->get_state() != &_kb) { hal::devices::kbd->attach_state(&_kb); }

            if (_ow != framebuffer.size().x || _oh != framebuffer.size().y)
            {
                _ow     = framebuffer.size().x;
                _oh     = framebuffer.size().y;
                int w   = (_ow - 2)  / _font.width();
                int h   = (_oh - 24) / _font.height();
                resize(w, h);
            }
        }

        void terminal_host::draw()
        {
            window::draw();
            framebuffer.copy(1, _tbar->info()->bounds.h + 1, &tbuff);
        }

        void terminal_host::render()
        {
            window::render();

            if (_cflash) { kernel::shell->framebuffer.rect_filled(_info.bounds.x + 1 + (_cx * _font.width()), _info.bounds.y + 23 + (_cy * _font.height()), _font.width(), _font.height(), _fg); }
        }

        void terminal_host::on_char(terminal_host* sender, void* arg)
        {
            if (sender == NULL) { return; }
            uint32_t c = (uint32_t)arg;
            sender->write(c);
        }

        void terminal_host::on_backspace(terminal_host* sender, void* arg)
        {
            if (sender == NULL) { return; }
            sender->backspace();
        }

        void terminal_host::on_enter(terminal_host* sender, void* arg)
        {
            if (sender == NULL) { return; }
            sender->print("\nYou typed: ");
            sender->println(sender->_kb.instream.ptr());
            sender->_kb.instream.clear();
            sender->print_caret();
        }

        void terminal_host::print_caret()
        {
            print("root", (uint32_t)std::color32::lime);
            write('@');
            print(_dir, (uint32_t)std::color32::magenta);
            print(": ");
        }

        void terminal_host::newline(int count)
        {
            _cx = 0; _cy++;
            if (_cy >= _ycount) { scroll(); }
        }

        void terminal_host::backspace(int count)
        {
            if (_cx > 0)
            {
                set_cursor_x(_cx - 1);
                putc(_cx, _cy, 0x20, _fg, _bg);
            }
            else if (_cy > 0)
            {
                set_cursor(_xcount - 1, _cy - 1);
                putc(_cx, _cy, 0x20, _fg, _bg);
            }
        }

        void terminal_host::scroll(int count)
        {
            uint32_t line = (tbuff.size().x * _font.height() * 4);
            uint32_t size = (tbuff.size().x * tbuff.size().y * 4);
            memcpy(tbuff.data().ptr(), (uint32_t*)((uint32_t)tbuff.data().ptr() + line), size - line);
            memset((uint32_t*)((uint32_t)tbuff.data().ptr() + (size - line)), (uint32_t)_bg, line);
            set_cursor(0, _ycount - 1);
        }

        void terminal_host::resize(int w, int h)
        {
            if (w == _xcount && h == _ycount) { return; }
            _xcount = w; _ycount = h;
            tbuff.dispose();
            tbuff = std::gfx::image(w * _font.width(), h * _font.height());
            clear();
            print_caret();
        }

        void terminal_host::clear() { clear(_bg); }

        void terminal_host::clear(uint32_t color)
        {
            _bg = color;
            tbuff.clear(color);
            set_cursor(0, 0);
        }

        void terminal_host::putc(int x, int y, char c, uint32_t fg, uint32_t bg)
        {
            if ((uint32_t)x >= _xcount || (uint32_t)y >= _ycount) { return; }
            tbuff.putc(x * _font.width(), y * _font.height(), c, _font, fg, bg);
            draw();
        }

        void terminal_host::putstr(int x, int y, char* str, uint32_t fg, uint32_t bg)
        {
            if (str == NULL || strlen(str) == 0) { return; }
            if ((uint32_t)x >= _xcount || (uint32_t)y >= _ycount) { return; }
            register int i = 0;
            int xx = x, yy = y;
            while (str[i] != 0)
            {
                if (str[i] == '\n') { xx = x; yy++; }
                else if (isprint(str[i]))
                {
                    putc(xx, yy, str[i], fg, bg);
                    xx++;
                    if (xx >= _xcount) { xx = x; yy++; }
                }
            }
        }
        
        void terminal_host::write(char c) { write(c, _fg, _bg); }

        void terminal_host::write(char c, uint32_t fg) { write(c, fg, _bg); }

        void terminal_host::write(char c, uint32_t fg, uint32_t bg)
        {
            if (c == '\n') { newline(); }
            else
            {
                putc(_cx, _cy, c, fg, bg);
                _cx++;
                if (_cx >= _xcount) { newline(); }
            }
        }

        void terminal_host::print(char* str) { print(str, _fg, _bg); }

        void terminal_host::print(char* str, uint32_t fg) { print(str, fg, _bg); }

        void terminal_host::print(char* str, uint32_t fg, uint32_t bg)
        {
            if (str == NULL || strlen(str) == 0) { return; }
            register int i = 0;
            while (str[i] != 0) { write(str[i], fg, bg); i++; }
        }

        void terminal_host::println(char* str) { println(str, _fg, _bg); }

        void terminal_host::println(char* str, uint32_t fg) { println(str, fg, _bg); }

        void terminal_host::println(char* str, uint32_t fg, uint32_t bg)
        {
            print(str, fg, bg);
            newline();
        }
        
        void terminal_host::set_colors(uint32_t fg, uint32_t bg) { _fg = fg; _bg = bg; }

        void terminal_host::set_forecolor(uint32_t color) { _fg = color; draw(); }

        void terminal_host::set_backcolor(uint32_t color) { _bg = color; draw(); }

        uint32_t terminal_host::forecolor() { return _fg; }

        uint32_t terminal_host::backcolor() { return _bg; }

        void terminal_host::set_cursor(int x, int y) { _cx = x; _cy = y; draw(); }
        void terminal_host::set_cursor_x(int x) { _cx = x; draw(); }
        void terminal_host::set_cursor_y(int y) { _cy = y; draw(); }

        std::ivec2d_t terminal_host::cursor() { return std::ivec2d_t(_cx, _cy); }

        void terminal_host::set_font(std::gfx::bitfont font) { _font = font; draw(); }

        std::gfx::bitfont* terminal_host::font() { return &_font; }
    }
}