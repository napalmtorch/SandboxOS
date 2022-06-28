#include <core/system/assets.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        namespace assets
        {
            std::gfx::bitfont font_square;
            std::gfx::bitfont font_comic;
            std::gfx::bitfont font_thin;

            std::gfx::image sys_icons;
            std::gfx::image img_logo;
            std::gfx::image bg_default;
            std::gfx::image tbar_icons;
            std::gfx::image mscur_default;
            std::gfx::image mscur_hand;
            std::gfx::image mscur_grab;
            std::gfx::image mscur_caret;
            std::gfx::image mscur_wait;

            std::gfx::bitfont load_font(const char* fname, int w, int h, int sx, int sy)
            {
                file_t* font_file = filesystem::open_file(fname, "r");
                if (font_file != NULL)
                {
                    uint8_t* data = new uint8_t[font_file->data.length()];
                    memcpy(data, font_file->data.ptr(), font_file->data.length());
                    filesystem::close_file(font_file);
                    std::gfx::bitfont fnt = std::gfx::bitfont(w, h, sx, sy, data);
                    printf("%s Loaded font at '%s'\n", DEBUG_INFO, fname);
                    return fnt;
                }
                else { printf("%s Failed to load font at '%s'\n", DEBUG_ERROR, fname); return std::gfx::bitfont(); }
            }

            void load()
            {
                font_square = load_font("A:/font.bin", 8, 12, 1, 1);
                font_comic  = load_font("A:/font_comic.bin", 8, 16, 1, 0);
                font_thin   = load_font("A:/font_thin.bin", 8, 14, 1, 0);

                if (font_thin.data() != NULL && font_thin.width(false) > 0) { std::FONT_DEFAULT = font_thin; }
                else { std::FONT_DEFAULT = std::gfx::bitfont(8, 14, 1, 0, (uint8_t*)std::FONTDATA_DEFAULT); }

                sys_icons     = std::gfx::image("A:/sysicons.bmp");
                img_logo      = std::gfx::image("A:/logo.bmp");
                bg_default    = std::gfx::image("A:/bg.bmp");
                tbar_icons    = std::gfx::image("A:/tbar.bmp");
                mscur_default = std::gfx::image("A:/mscur_default.bmp");
                mscur_grab    = std::gfx::image("A:/mscur_grab.bmp");
                mscur_caret   = std::gfx::image("A:/mscur_caret.bmp");
                mscur_hand    = std::gfx::image("A:/mscur_hand.bmp");
                mscur_wait    = std::gfx::image("A:/mscur_wait.bmp");

                printf("%s Finished loading assets\n", DEBUG_OK);
            }
        }
    }
}