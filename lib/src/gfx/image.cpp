#include <gfx/image.h>
#include <core/kernel.h>

namespace std
{
    namespace gfx
    {
        image::image() { load(0, 0, array<uint32_t>()); }

        image::image(int w, int h) { load(w, h, array<uint32_t>(w * h)); }

        image::image(ivec2d_t size) { load(size.x, size.y, array<uint32_t>(size.x * size.y)); }
    
        image::image(int w, int h, array<uint32_t> data) { load(w, h, data); }

        image::image(ivec2d_t size, array<uint32_t> data) { load(size.x, size.y, data); }

        image::image(char* fname)
        {
            load(0, 0, array<uint32_t>());
            load(fname);
        }

        void image::dispose()
        {
            _data.dispose();
            _size = ivec2d_t(0, 0);
        }

        bool image::load(int w, int h, array<uint32_t> data)
        {
            this->_size  = ivec2d_t(w, h);
            this->_data = data;
            return true;
        }

        bool image::load(char* fname)
        {
            image img = os::filesystem::parsing::bmp_load(fname);
            load(img.size().x, img.size().y, img.data());
            if (_data.ptr() == NULL) { return false; }
            return true;
        }

        void image::resize(int w, int h)
        {
            if (_data.ptr() == NULL) { return; }
            array<uint32_t> temp(w * h);
            double x_ratio = (double)_size.x / (double)w;
            double y_ratio = (double)_size.y / (double)h;
            int px, py;
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    px = floor(j * x_ratio);
                    py = floor(i * y_ratio);
                    temp[(i * w) + j] = ((uint32_t*)_data.ptr())[(int)((py * _size.x) + px)];
                }
            }
            _data.dispose();
            _data = temp;
            _size = ivec2d_t(w, h);
        }

        void image::resize(ivec2d_t size) { resize(size.x, size.y); }

        void image::clear(uint32_t color)
        {
            if (_data.ptr() == NULL) { return; }
            memset(_data.ptr(), color, _data.length() * 4);
        }

        void image::clear(color32 color) { clear((uint32_t)color); }

        void image::blit(int x, int y, uint32_t color)
        {
            if ((uint32_t)x >= (uint32_t)_size.x || (uint32_t)y >= (uint32_t)_size.y) { return; }
            _data[y * _size.x + x] = color;
        }

        void image::blit(int x, int y, color32 color) { blit(x, y, (uint32_t)color); }

        void image::blit(ivec2d_t pos, uint32_t color) { blit(pos.x, pos.y, color); }

        void image::blit(ivec2d_t pos, color32 color) { blit(pos.x, pos.y, (uint32_t)color); }

        void image::rect_filled(int x, int y, int w, int h, uint32_t color)
        {
            for (int i = 0; i < w * h; i++) { blit(x + (i % w), y + (i / w), color); }
        }

        void image::rect_filled(int x, int y, int w, int h, color32 color) { rect_filled(x, y, w, h, (uint32_t)color); }

        void image::rect_filled(ivec2d_t pos, ivec2d_t size, uint32_t color) { rect_filled(pos.x, pos.y, size.x, size.y, color); }

        void image::rect_filled(ivec2d_t pos, ivec2d_t size, color32 color) { rect_filled(pos.x, pos.y, size.x, size.y, (uint32_t)color); }

        void image::rect_filled(irect_t bounds, uint32_t color) { rect_filled(bounds.x, bounds.y, bounds.w, bounds.h, color); }

        void image::rect_filled(irect_t bounds, color32 color) { rect_filled(bounds.x, bounds.y, bounds.w, bounds.h, (uint32_t)color); }

        void image::rect(int x, int y, int w, int h, int t, uint32_t color)
        {
            if (color == (uint32_t)color32::transparent) { return; }
            rect_filled(x, y, w, t, color);
            rect_filled(x, y + h - t, w, t, color);
            rect_filled(x, y + t, t, h - (t * 2), color);
            rect_filled(x + w - t, y + t, t, h - (t * 2), color);
        }

        void image::rect(int x, int y, int w, int h, int t, color32 color) { rect(x, y, w, h, t, (uint32_t)color); }

        void image::rect(ivec2d_t pos, ivec2d_t size, int t, uint32_t color) { rect(pos.x, pos.y, size.x, size.y, t, color); }

        void image::rect(ivec2d_t pos, ivec2d_t size, int t, color32 color) { rect(pos.x, pos.y, size.x, size.y, t, (uint32_t)color); }

        void image::rect(irect_t bounds, int t, uint32_t color) { rect(bounds.x, bounds.y, bounds.w, bounds.h, t, color); }

        void image::rect(irect_t bounds, int t, color32 color) { rect(bounds.x, bounds.y, bounds.w, bounds.h, t, (uint32_t)color); }

        void image::putc(int x, int y, char c, bitfont font, uint32_t fg, uint32_t bg)
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

        void image::putc(int x, int y, char c, bitfont font, color32 fg, color32 bg) { putc(x, y, c, font, (uint32_t)fg, (uint32_t)bg); }

        void image::putc(ivec2d_t pos, char c, bitfont font, uint32_t fg, uint32_t bg) { putc(pos.x, pos.y, c, font, fg, bg); }

        void image::putc(ivec2d_t pos, char c, bitfont font, color32 fg, color32 bg) { putc(pos.x, pos.y, c, font, (uint32_t)fg, (uint32_t)bg); }

        void image::putstr(int x, int y, char* str, bitfont font, uint32_t fg, uint32_t bg)
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

        void image::putstr(int x, int y, char* str, bitfont font, color32 fg, color32 bg) { putstr(x, y, str, font, (uint32_t)fg, (uint32_t)bg); }

        void image::putstr(ivec2d_t pos, char* str, bitfont font, uint32_t fg, uint32_t bg) { putstr(pos.x, pos.y, str, font, fg, bg); }

        void image::putstr(ivec2d_t pos, char* str, bitfont font, color32 fg, color32 bg) { putstr(pos.x, pos.y, str, font, (uint32_t)fg, (uint32_t)bg); }

        void image::copy(int x, int y, int sw, int sh, uint32_t* data)
        {
            if (sw == _size.x && sh == _size.y) { memcpy((void*)_data.ptr(), data, sw * sh * 4); return; }

            uint32_t iw = sw;
            while (x + sw > (int)_size.x) { sw--; }
            while (y + sh > (int)_size.y) { sh--; }

            for (int yy = 0; yy < sh; yy++)
            {
                uint8_t* src = (uint8_t*)(data + (yy * iw));
                int xx = x;
                if (xx < 0) { xx = 0; }
                while (xx + sw > (int)_size.x) { xx--; }
                uint32_t ro = (xx + ((y + yy) * _size.x)) * 4;
                uint8_t* dest = (uint8_t*)((uint32_t)_data.ptr() + ro);
                if (y + yy >= (int)_size.y) { return; }
                if (y + yy >= 0 && (uint32_t)dest >= (uint32_t)_data.ptr() && (uint32_t)dest < (uint32_t)((uint32_t)_data.ptr() + (_size.x * _size.y * 4)))
                {
                    if (x >= 0) { memcpy(dest, src, sw * 4); }
                    else { memcpy(dest, src - (x * 4), (sw + x) * 4); }
                }
            }       
        }

        void image::copy(int x, int y, int sw, int sh, color32* data) { copy(x, y, sw, sh, (uint32_t*)data); }

        void image::copy(ivec2d_t pos, ivec2d_t size, uint32_t* data) { copy(pos.x, pos.y, size.x, size.y, data); }

        void image::copy(ivec2d_t pos, ivec2d_t size, color32* data) { copy(pos.x, pos.y, size.x, size.y, (uint32_t*)data); }        

        void image::copy(irect_t bounds, uint32_t* data) { copy(bounds.x, bounds.y, bounds.w, bounds.h, data); }

        void image::copy(irect_t bounds, color32* data) { copy(bounds.x, bounds.y, bounds.w, bounds.h, (uint32_t*)data); }

        void image::copy(int x, int y, image* img) { copy(x, y, img->size().x, img->size().y, img->data().ptr()); }

        void image::copy(ivec2d_t pos, image* img) { copy(pos.x, pos.y, img->size().x, img->size().y, img->data().ptr()); }

        void image::copy(int x, int y, int sw, int sh, uint32_t trans, uint32_t* data)
        {
            for (size_t i = 0; i < sw * sh; i++)
            {
                if (data[i] == trans) { continue; }
                blit(x + (i % sw), y + (i / sw), data[i]);
            }
        }

        void image::copy(int x, int y, int sw, int sh, color32 trans, color32* data) { copy(x, y, sw, sh, (uint32_t)trans, (uint32_t*)data); }

        void image::copy(ivec2d_t pos, ivec2d_t size, uint32_t trans, uint32_t* data) { copy(pos.x, pos.y, size.x, size.y, trans, data); }

        void image::copy(ivec2d_t pos, ivec2d_t size, color32 trans, color32* data) { copy(pos.x, pos.y, size.x, size.y, (uint32_t)trans, (uint32_t*)data); }

        void image::copy(irect_t bounds, uint32_t trans, uint32_t* data) { copy(bounds.x, bounds.y, bounds.w, bounds.h, trans, data); }

        void image::copy(irect_t bounds, color32 trans, color32* data) { copy(bounds.x, bounds.y, bounds.w, bounds.h, (uint32_t)trans, (uint32_t*)data); }

        void image::copy(int x, int y, uint32_t trans, image* img) { copy(x, y, img->size().x, img->size().y, trans, img->data().ptr()); }

        void image::copy(int x, int y, color32 trans, image* img) { copy(x, y, img->size().x, img->size().y, (uint32_t)trans, img->data().ptr()); }

        void image::copy(ivec2d_t pos, uint32_t trans, image* img) { copy(pos.x, pos.y, img->size().x, img->size().y, trans, img->data().ptr()); }

        void image::copy(ivec2d_t pos, color32 trans, image* img) { copy(pos.x, pos.y, img->size().x, img->size().y, (uint32_t)trans, img->data().ptr()); }

        void image::copy(int x, int y, irect_t src, uint32_t trans, image* img)
        {
            if (img == NULL) { return; }
            for (int i = 0; i < src.w * src.h; i++)
            {
                if (img->data()[(src.y + (i / src.w)) * img->size().x + (src.x + (i % src.w))] != (uint32_t)trans) { blit(x + (i % src.w), y + (i / src.w), img->data()[(src.y + (i / src.w)) * img->size().x + (src.x + (i % src.w))]); }
            }
        }

        void image::copy(int x, int y, irect_t src, color32 trans, image* img)
        {
            if (img == NULL) { return; }
            for (int i = 0; i < src.w * src.h; i++)
            {
                if (img->data()[(src.y + (i / src.w)) * img->size().x + (src.x + (i % src.w))] != (uint32_t)trans) { blit(x + (i % src.w), y + (i / src.w), img->data()[(src.y + (i / src.w)) * img->size().x + (src.x + (i % src.w))]); }
            }
        }

        ivec2d_t image::size() { return _size; }

        array<uint32_t> image::data() { return _data; }
    }
}