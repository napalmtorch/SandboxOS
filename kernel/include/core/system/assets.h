#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <gfx/image.h>
#include <gfx/font.h>

namespace os
{
    namespace sys
    {
        enum class font_id : uint32_t
        {
            comic_8x16 = 0xA0000000,
            square_8x12,
            thin_8x12,
            thin_8x14,
        };

        enum class image_id : uint32_t
        {
            logo = 0xF0000000,
            default_bg,
            icons,
            icons_tray,
            icons_tbar,
            ms_default,
            ms_hand,
            ms_wait,
            ms_grab,
            ms_caret,
        };

        class asset_manager
        {
            private:
                static std::arraylist<std::gfx::bitfont> _fonts;
                static std::arraylist<std::gfx::image>   _images;

            public:
                static void init();

            public:
                static std::gfx::bitfont* load_font(const char* fname, int w, int h, int sx, int sy, font_id id);
                static std::gfx::image*   load_image(const char* fname, image_id id);

            public:
                static std::gfx::bitfont* fetch_font(font_id id);
                static std::gfx::image*   fetch_image(image_id id);

            public:
                static bool id_used(uint32_t id);
        };
    }
}