#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <array.h>
#include <gfx/color.h>
#include <gfx/font.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    namespace gfx
    {
        class image
        {
            private:
                ivec2d_t _size;
                std::array<uint32_t> _data;

            public:
                image();
                image(int w, int h);
                image(ivec2d_t size);
                image(int w, int h, std::array<uint32_t> data);
                image(ivec2d_t size, std::array<uint32_t> data);
                image(char* fname);

                void dispose();

                bool load(int w, int h, std::array<uint32_t> data);
                bool load(char* fname);

                void resize(int w, int h);
                void resize(ivec2d_t size);

                void clear(uint32_t color = 0xFF000000);
                void clear(color32 color);

                void blit(int x, int y, uint32_t color = 0xFF000000);
                void blit(int x, int y, color32 color);
                void blit(ivec2d_t pos, uint32_t color = 0xFF000000);
                void blit(ivec2d_t pos, color32 color);

                void rect_filled(int x, int y, int w, int h, uint32_t color = 0xFF000000);
                void rect_filled(int x, int y, int w, int h, color32 color);
                void rect_filled(ivec2d_t pos, ivec2d_t size, uint32_t color = 0xFF000000);
                void rect_filled(ivec2d_t pos, ivec2d_t size, color32 color);
                void rect_filled(irect_t bounds, uint32_t color = 0xFF000000);
                void rect_filled(irect_t bounds, color32 color);

                void rect(int x, int y, int w, int h, int t, uint32_t color = 0xFF000000);
                void rect(int x, int y, int w, int h, int t, color32 color);
                void rect(ivec2d_t pos, ivec2d_t size, int t, uint32_t color = 0xFF000000);
                void rect(ivec2d_t pos, ivec2d_t size, int t, color32 color);
                void rect(irect_t bounds, int t, uint32_t color = 0xFF000000);
                void rect(irect_t bounds, int t, color32 color);

                void putc(int x, int y, char c, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                void putc(int x, int y, char c, bitfont font, color32 fg, color32 bg);
                void putc(ivec2d_t pos, char c, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                void putc(ivec2d_t pos, char c, bitfont font, color32 fg, color32 bg);

                void putstr(int x, int y, char* str, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                void putstr(int x, int y, char* str, bitfont font, color32 fg, color32 bg);
                void putstr(ivec2d_t pos, char* str, bitfont font, uint32_t fg, uint32_t bg = 0x00FFFFFF);
                void putstr(ivec2d_t pos, char* str, bitfont font, color32 fg, color32 bg);

                void copy(int x, int y, int sw, int sh, uint32_t* data);
                void copy(int x, int y, int sw, int sh, color32* data);
                void copy(ivec2d_t pos, ivec2d_t size, uint32_t* data);
                void copy(ivec2d_t pos, ivec2d_t size, color32* data);
                void copy(irect_t bounds, uint32_t* data);
                void copy(irect_t bounds, color32* data);
                void copy(int x, int y, image* img);
                void copy(ivec2d_t pos, image* img);

                void copy(int x, int y, int sw, int sh, uint32_t trans, uint32_t* data);
                void copy(int x, int y, int sw, int sh, color32 trans, color32* data);
                void copy(ivec2d_t pos, ivec2d_t size, uint32_t trans, uint32_t* data);
                void copy(ivec2d_t pos, ivec2d_t size, color32 trans, color32* data);
                void copy(irect_t bounds, uint32_t trans, uint32_t* data);
                void copy(irect_t bounds, color32 trans, color32* data);
                void copy(int x, int y, uint32_t trans, image* img);
                void copy(int x, int y, color32 trans, image* img);
                void copy(ivec2d_t pos, uint32_t trans, image* img);
                void copy(ivec2d_t pos, color32 trans, image* img);
        };
    }
}