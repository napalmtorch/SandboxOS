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

            extern std::gfx::image sys_icons;
            extern std::gfx::image img_logo;
            extern std::gfx::image bg_default;
            extern std::gfx::image tbar_icons;
            extern std::gfx::image mscur_default;
            extern std::gfx::image mscur_hand;
            extern std::gfx::image mscur_grab;
            extern std::gfx::image mscur_caret;
            extern std::gfx::image mscur_wait;

            void load();
        }
    }
}