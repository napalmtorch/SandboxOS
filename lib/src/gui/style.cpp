#include <gui/style.h>
#include <core/kernel.h>

namespace std
{
    namespace gui
    {
        namespace default_styles
        {
            visual_style* CONTAINER;
            visual_style* WINDOW;
            visual_style* BUTTON;
            visual_style* TEXTBOX;

            void init()
            {
                uint32_t colors[] = 
                { 
                    0xFF202020, 0xFF404040, 0xFF303030, 0xFF404040, 0xFF000000, 
                    0xFFBFBFBF, 0xFFFFFFFF, 0xFFDFDFDF, 0xFF7F7F7F, 0xFF000000, 
                    0xFF7F7F7F, 0xFFAFAFAF, 0xFF404040, 0xFF303030, 
                    0xFF303030, 0xFF404040, 0xFF7F7F7F, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000 
                };
                
                CONTAINER = new visual_style("default", colors, 1, false, false, FONT_DEFAULT);
                WINDOW    = new visual_style("default", colors, 1, false, false, FONT_DEFAULT);
                BUTTON    = NULL;
                TEXTBOX   = NULL;
                printf("%s Initialized default visual style\n", DEBUG_OK);
            }
        }

        visual_style::visual_style()
        {
            memset(this->_name, 0, sizeof(this->_name));
            memset(this->_colors, 0, sizeof(this->_colors));
            this->_border_size = 0;
            this->_shadow      = false;
            this->_text_shadow = false;
        }

        visual_style::visual_style(char* name, uint32_t* colors, uint32_t border, bool shad, bool txt_shad, gfx::bitfont font)
        {
            strcpy(this->_name, name);
            memcpy(this->_colors, colors, sizeof(this->_colors));
            this->_border_size = border;
            this->_shadow      = shad;
            this->_text_shadow = txt_shad;
            this->_font        = font;
        }

        void visual_style::set_color(int index, uint32_t color)
        {
            if (index >= (int)color_index::count) { perror("Style index out of bounds"); return; }
            _colors[index] = color;
        }

        void visual_style::set_color(color_index index, uint32_t color)
        {
            if (index >= color_index::count) { perror("Style index out of bounds"); return; }
            _colors[(int)index] = color;
        }

        void visual_style::set_bordersz(uint32_t border) { _border_size = border; }

        void visual_style::set_font(gfx::bitfont font) { _font = font; }

        void visual_style::enable_shadow() { _shadow = true; }

        void visual_style::disable_shadow() { _shadow = false; }

        void visual_style::enable_txtshadow() { _text_shadow = true; }

        void visual_style::disable_txtshadow() { _text_shadow = false; }

        uint32_t visual_style::color(int index)
        {
            if (index >= (int)color_index::count) { perror("Style index out of bounds"); return 0; }
            return _colors[index];
        }

        uint32_t visual_style::color(color_index index)
        {
            if (index >= color_index::count) { perror("Style index out of bounds"); return 0; }
            return _colors[(int)index];
        }
        
        char* visual_style::name() { return _name; }

        uint32_t* visual_style::colors() { return _colors; }

        bool visual_style::shadow() { return _shadow; }

        bool visual_style::txtshadow() { return _text_shadow; }

        bool visual_style::bordersz() { return _border_size; }

        gfx::bitfont visual_style::font() { return _font; }
    }
}