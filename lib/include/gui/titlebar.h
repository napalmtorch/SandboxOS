#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <gui/control.h>

namespace std
{
    namespace gui
    {
        class button;
        class window;

        class titlebar : public control
        {
            friend class window;

            public:
                button* btn_close;
                button* btn_max;
                button* btn_min;
            
            public:
                titlebar(window* parent);

            public:
                void dispose() override;
                void update()  override;
                void draw()    override;
                void render()  override;

            public:
                void handle_movement();
        };
    }
}