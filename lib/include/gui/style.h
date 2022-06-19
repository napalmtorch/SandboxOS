#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/image.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    namespace gui
    {
        enum class color_index : uint8_t
        {
            bg,
            bg_hover,
            bg_down,
            bg_disabled,
            bg_shadow,

            txt,
            txt_hover,
            txt_down,
            txt_disabled,
            txt_shadow,    

            border,
            border_hover,
            border_down,
            border_disabled,   

            top,
            top_down,
            top_inactive,
            toptxt,
            toptxt_down,
            toptxt_inactive,
            
            count,
        };

        class visual_style
        {
            private:
                char         _name[64];
                uint32_t     _colors[20];
                bool         _shadow;
                bool         _text_shadow;
                uint32_t     _border_size;
                gfx::bitfont _font;

            public:
                visual_style();
                visual_style(char* name, uint32_t* colors, uint32_t border, bool shad, bool txt_shad, gfx::bitfont font);

            public:
                void set_color(int index, uint32_t color);
                void set_color(color_index index, uint32_t color);
                void set_bordersz(uint32_t border);
                void set_font(gfx::bitfont font);
                void enable_shadow();
                void disable_shadow();
                void enable_txtshadow();
                void disable_txtshadow();

            public:
                uint32_t color(int index);
                uint32_t color(color_index index);
                
            public:
                char* name();
                uint32_t* colors();
                bool      shadow();
                bool      txtshadow();
                bool      bordersz();
                gfx::bitfont font();
        };

        namespace default_styles
        {
            extern visual_style* CONTAINER;
            extern visual_style* WINDOW;
            extern visual_style* BUTTON;
            extern visual_style* TEXTBOX;

            void init();
        }
    }
}