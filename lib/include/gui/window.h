#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <arraylist.h>
#include <gui/container.h>

namespace os::services { class window_manager; }

namespace std
{
    namespace gui
    {
        class titlebar;        

        enum class window_state : uint8_t
        {
            minimized,
            maximized,
            normal,
        };

        class window : public container
        {
            friend class titlebar;
            friend class os::services::window_manager;

            public:
                int          index;
                window_state state;

            protected:
                titlebar*   _tbar;
                bool        _moving, _resizing, _closed;
                std::irect_t _old_bounds;
            
            public:
                window(int x, int y, int w, int h, char* label);

            public:
                void dispose() override;
                void update() override;
                void draw() override;
                void render() override;
        };
    }
}