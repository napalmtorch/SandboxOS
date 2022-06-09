#include <hardware/devices/vbe.h>
#include <core/kernel.h>

#define VBE_CTRL_PTR 0x80000
#define LNG_PTR(seg, off) ((seg << 4) | off)
#define REAL_PTR(arr) LNG_PTR(arr[1], arr[0])
#define SEG(addr) (((uint32_t)addr >> 4) & 0xF000)
#define OFF(addr) ((uint32_t)addr & 0xFFFF)

namespace os
{
    namespace hal
    {         
        namespace devices
        {
            vbe_controller* vbe;
        }

        void vbe_controller::init()
        {
            device_t::init();
            populate_headers();
            set_info("VBE Graphics Controller", 0xD0000001);
        }
    
        void vbe_controller::start()
        {
            device_t::start();
            set_mode(kernel::start_args.screen_size.x, kernel::start_args.screen_size.y);
        }
        
        void vbe_controller::stop()
        {
            device_t::stop();
        }

        void vbe_controller::populate_headers()
        {
            memset(&_modeinfo, 0, sizeof(sys::vbe_mode_info_t));
            memset(&_ctrlinfo, 0, sizeof(sys::vbe_ctrl_info_t));
            sys::vbe_ctrl_info_t* info = (sys::vbe_ctrl_info_t*)VBE_CTRL_PTR;
            registers16_t regs;

            memset(&regs, 0, sizeof(registers16_t));
            regs.ax = 0x4F00;
            regs.es = 0x8000;
            regs.di = 0x0000;
            int32(0x10, &regs);

            if (regs.ax != 0x4F) { perror("Failed to initialize VBE controller"); return; }
            _ctrlinfo = *info;
            _modeinfo = *(sys::vbe_mode_info_t*)kernel::multiboot.vbe_mode_info;
            _size     = _modeinfo.width * _modeinfo.height * 4;
        }

        void vbe_controller::set_mode(int w, int h)
        {
            populate_headers();

            registers16_t regs;
            memset(&regs, 0, sizeof(registers16_t));
            sys::vbe_mode_info_t* minfo = (sys::vbe_mode_info_t*)(VBE_CTRL_PTR + sizeof(sys::vbe_ctrl_info_t) + 512);
            uint16_t* modes = (uint16_t*)REAL_PTR(_ctrlinfo.videomode);
            uint16_t mode;

            for (int i = 0; modes[i] != 0xFFFF; i++)
            {
                regs.ax = 0x4F01;
                regs.cx = modes[i];
                regs.es = SEG(minfo);
                regs.di = OFF(minfo);
                int32(0x10, &regs);

                if (minfo->width == w && minfo->height == h && minfo->depth == 32)
                {
                    mode = modes[i];
                    regs.ax = 0x4F02;
                    regs.bx = mode | 0x4000;
                    int32(0x10, &regs);
                    _modeinfo = *minfo;
                    _size      = _modeinfo.width * _modeinfo.height * 4;
                    printf("%s Set VBE mode to %dx%dx32bpp\n", DEBUG_INFO, _modeinfo.width, _modeinfo.height);
                    return;
                }
            }
        }

        void vbe_controller::clear(uint32_t color)
        {
            if (_modeinfo.physical_base < 0xF0000000) { return; }
            memset((void*)_modeinfo.physical_base, color, _size);
        }

        void vbe_controller::blit(int x, int y, uint32_t color)
        {
            if ((uint32_t)x >= _modeinfo.width || (uint32_t)y >+ _modeinfo.height) { return; }
            ((uint32_t*)_modeinfo.physical_base)[y * _modeinfo.width + x] = color;
        }

        void vbe_controller::rect_filled(int x, int y, int w, int h, uint32_t color)
        {
            for (int i = 0; i < w * h; i++) { blit(x + (i % w), y + (i / w), color); }
        }

        void vbe_controller::putc(int x, int y, char c, std::gfx::bitfont font, uint32_t fg, uint32_t bg)
        {
            if (bg != 0x00FFFFFF) { rect_filled(x, y, font.width(), font.height(), bg); }
            uint32_t p = font.height(false) * c;

            int i = 0;
            while (i < font.width(false) * font.height(false))
            {
                int xx = x + (font.width(false) - (i % font.width(false)));
                int yy = y + (i / font.width(false));
                if (bit_addr_from_byte(font.data()[p + (i / font.width(false))], (i % font.width(false)) + 1)) { blit(xx, yy, fg); }
                i++;
            }
        }

        void vbe_controller::putstr(int x, int y, char* str, std::gfx::bitfont font, uint32_t fg, uint32_t bg)
        {
            if (str == NULL) { return; }

            int xx = x, yy = y, i = 0;
            while (str[i] != 0)
            {
                if (str[i] == '\n') { xx = x; yy += font.height(); }
                else { putc(xx, yy, str[i], font, fg, bg); xx += font.width(); }
                i++;
            }
        }

        void vbe_controller::copy(int x, int y, int w, int h, uint32_t* data)
        {
            if (data == NULL) { return; }
            if (w == _modeinfo.width && h == _modeinfo.height) { memcpy((void*)_modeinfo.physical_base, data, w * h * 4); return; }

            while (x + w > (int)_modeinfo.width) { w--; }
            while (y + h > (int)_modeinfo.height) { h--; }

            for (int yy = 0; yy < h; yy++)
            {
                uint8_t* src = (uint8_t*)(data + (yy * w));
                int xx = x;
                if (xx < 0) { xx = 0; }
                while (xx + w > (int)_modeinfo.width) { xx--; }
                uint32_t ro = (xx + ((y + yy) * _modeinfo.width)) * 4;
                uint8_t* dest = (uint8_t*)((uint32_t)_modeinfo.physical_base + ro);
                if (y + yy >= (int)_modeinfo.height) { return; }
                if (y + yy >= 0 && (uint32_t)dest >= (uint32_t)_modeinfo.physical_base && (uint32_t)dest < (uint32_t)((uint32_t)_modeinfo.physical_base + _size))
                {
                    if (x >= 0) { memcpy(dest, src, w * 4); }
                    else { memcpy(dest, src - (x * 4), (w + x) * 4); }
                }
            }
        }

        uint32_t vbe_controller::get_pixel(int x, int y)
        {
            return ((uint32_t*)_modeinfo.physical_base)[y * _modeinfo.width + x];   
        }

        sys::vbe_mode_info_t vbe_controller::modeinfo() { return _modeinfo; }
    }
}