#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/image.h>
#include <gfx/font.h>

namespace os
{
    namespace sys
    {
        namespace assets
        {
            extern std::gfx::bitfont font_square;
            extern std::gfx::bitfont font_comic;
            extern std::gfx::bitfont font_thin;

            extern std::gfx::image img_logo;

            void load();
        }
    }
}