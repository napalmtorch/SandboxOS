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

            std::gfx::image img_logo;

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
                font_square = load_font("A:/font.bin", 8, 14, 1, 0);
                font_comic  = load_font("A:/font_comic.bin", 8, 16, 1, 0);
                font_thin   = load_font("A:/font_thin.bin", 8, 14, 1, 0);
                img_logo    = std::gfx::image("A:/logo.bmp");

                if (font_thin.data() != NULL && font_thin.width(false) > 0)
                {
                    std::FONT_DEFAULT = font_thin;
                }
                else { std::FONT_DEFAULT = std::gfx::bitfont(8, 14, 1, 0, (uint8_t*)std::FONTDATA_DEFAULT); }

                printf("%s Finished loading assets\n", DEBUG_OK);
            }
        }
    }
}