#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <gfx/font.h>
#include <gfx/color.h>
#include <gfx/image.h>
#include <gfx/primitives/rectangle.h>
#include <gfx/primitives/vec2d.h>
#include <gui/style.h>

namespace std
{
    namespace gui
    {
        class control;
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
            bool tooltip;
        } PACKED control_flags_t;

        typedef struct
        {
            irect_t       bounds;
            char*         label;
            char*         tooltip_txt;
            container*    parent;
            visual_style* style;
        } control_info_t;

        typedef struct
        {
            void (*on_click)(control* sender, void* arg);
            void (*on_ms_down)(control* sender, void* arg);
            void (*on_ms_up)(control* sender, void* arg);
            void (*on_ms_enter)(control* sender, void* arg);
            void (*on_ms_leave)(control* sender, void* arg);
            void (*on_ms_hover)(control* sender, void* arg);
        } control_events_t;

        enum class control_type : uint8_t
        {
            base,
            container,
            window,
            titlebar,
            button,
        };

        class control
        {
            protected:
                control_info_t   _info;
                control_flags_t  _flags;
                control_events_t _events;
                control_type     _type;
                
            public:
                control(int x, int y, int w, int h, char* label, control_type type, container* parent = NULL);

            public:
                virtual void dispose();
                virtual void update();
                virtual void draw();
                virtual void render();

            protected:
                void update_events();

            public:
                void set_label(char* label);

            public:
                char*             label();
                container*        parent();
                irect_t*          bounds();
                irect_t           screen_bounds();
                control_info_t*   info();
                control_flags_t*  flags();
                control_events_t* events();
                control_type      type();
        };
    }
}