#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <gfx/font.h>
#include <gfx/color.h>
#include <gfx/image.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>

namespace std
{
    namespace gui
    {
        class container;

        typedef struct
        {
            bool enabled;
            bool visible;
            bool hover;
            bool down;
            bool clicked;
            bool unclicked;
            bool toggled;
            bool active;
        } PACKED control_flags_t;

        typedef struct
        {
            irect_t    bounds;
            char*      label;
            container* parent;
        } control_info_t;

        class control
        {
            protected:
                control_info_t  _info;
                control_flags_t _flags;

            public:
                control(int x, int y, int w, int h, char* label, container* parent = NULL);

            public:
                virtual void dispose();
                virtual void update();
                virtual void draw();
                virtual void render();

            public:
                void set_label(char* label);

            public:
                char*            label();
                void*            parent();
                irect_t*         bounds();
                control_info_t*  info();
                control_flags_t* flags();
        };
    }
}