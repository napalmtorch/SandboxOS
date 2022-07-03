#include <core/system/assets.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        std::arraylist<std::gfx::bitfont> asset_manager::_fonts;
        std::arraylist<std::gfx::image>   asset_manager::_images;

        void asset_manager::init()
        {
            _fonts  = std::arraylist<std::gfx::bitfont>();
            _images = std::arraylist<std::gfx::image>();


            load_font("A:/font_comic16.bin", 8, 16, 1, 0, font_id::comic_8x16);
            load_font("A:/font_square12.bin", 8, 12, 1, 0, font_id::square_8x12);
            load_font("A:/font_thin12.bin", 8, 12, 1, 0, font_id::thin_8x12);
            load_font("A:/font_thin14.bin", 8, 14, 1, 0, font_id::thin_8x14);

            load_image("A:/logo.bmp", image_id::logo);
            load_image("A:/sysicons.bmp", image_id::icons_tray);
            load_image("A:/icons32.bmp", image_id::icons);
            load_image("A:/tbar.bmp", image_id::icons_tbar);
            load_image("A:/bg.bmp", image_id::default_bg);
            load_image("A:/mscur_caret.bmp", image_id::ms_caret);
            load_image("A:/mscur_default.bmp", image_id::ms_default);
            load_image("A:/mscur_grab.bmp", image_id::ms_grab);
            load_image("A:/mscur_hand.bmp", image_id::ms_hand);
            load_image("A:/mscur_wait.bmp", image_id::ms_wait);


            if (fetch_font(font_id::thin_8x12) != NULL) { std::FONT_DEFAULT = *fetch_font(font_id::thin_8x12); }
            else { std::FONT_DEFAULT = std::gfx::bitfont(8, 14, 1, 0, (uint8_t*)std::FONTDATA_DEFAULT); }

            printf("%s Initialized asset manager\n", DEBUG_OK);
        }

        std::gfx::bitfont* asset_manager::load_font(const char* fname, int w, int h, int sx, int sy, font_id id)
        {
            if (id_used((uint32_t)id)) { perror("Asset with id %u already exists", id); return NULL; }
            file_t* font_file = filesystem::open_file(fname, "r");
            if (font_file != NULL)
            {
                uint8_t* data = new uint8_t[font_file->data.length()];
                memcpy(data, font_file->data.ptr(), font_file->data.length());
                filesystem::close_file(font_file);
                std::gfx::bitfont font = std::gfx::bitfont(w, h, sx, sy, data);
                font.set_id((uint32_t)id);
                _fonts.add(font);
                printf("%s Loaded font(0x%8x) at '%s'\n", DEBUG_INFO, font.id(), fname);
                return &_fonts[_fonts.length() - 1];
            }
            else { printf("%s Failed to load font at '%s'\n", DEBUG_ERROR, fname); return NULL; }
        }

        std::gfx::image* asset_manager::load_image(const char* fname, image_id id)
        {
            if (id_used((uint32_t)id)) { perror("Asset with id %u already exists", id); return NULL; }
            std::gfx::image img = std::gfx::image((char*)fname);
            if (img.data().ptr() == NULL) { printf("%s Failed to load image at '%s'\n", DEBUG_ERROR, img.id(), fname); return NULL; }
            img.set_id((uint32_t)id);
            _images.add(img);
            printf("%s Loaded image(0x%8x) at '%s'\n", DEBUG_INFO, fname);
            return &_images[_images.length() - 1];
        }

        std::gfx::bitfont* asset_manager::fetch_font(font_id id)
        {
            for (size_t i = 0; i < _fonts.length(); i++)
            {
                if (_fonts[i].id() == (uint32_t)id) { return &_fonts[i]; }
            }
            return NULL;
        }

        std::gfx::image* asset_manager::fetch_image(image_id id)
        {
            for (size_t i = 0; i < _images.length(); i++)
            {
                if (_images[i].id() == (uint32_t)id) { return &_images[i]; }
            }
            return NULL;
        }

        bool asset_manager::id_used(uint32_t id)
        {
            if (fetch_font((font_id)id)   != NULL) { return true; }
            if (fetch_image((image_id)id) != NULL) { return true; }
            return false;
        }
    }
}