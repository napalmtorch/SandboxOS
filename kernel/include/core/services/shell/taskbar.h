#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <gui/container.h>
#include <gui/button.h>

namespace os
{
    namespace services
    {
        class shell_host;

        class shell_taskbar : public std::gui::container
        {
            public:
                shell_host*       host;
                std::gui::button* btn_menu;

            private:
                int _now, _last;
                char timestr[64];

            public:
                shell_taskbar(shell_host* host);

            public:
                void dispose() override;
                void update() override;
                void draw() override;
                void render() override;
        };
    }
}