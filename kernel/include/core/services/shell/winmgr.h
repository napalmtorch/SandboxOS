#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <arraylist.h>
#include <gui/window.h>

namespace os
{
    namespace services
    {
        class window_manager
        {
            friend class window;

            public:
                std::arraylist<std::gui::window*> windows;
                std::gui::window*                 active_win;

            public:
                void init();
                void update();
                void draw();

            public:
                void load(std::gui::window* win);
                void unload(std::gui::window* win);

            public:
                std::gui::window* set_active(std::gui::window* win);
        };
    }
}