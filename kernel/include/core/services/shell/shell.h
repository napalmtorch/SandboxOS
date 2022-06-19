#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/image.h>
#include <core/exec/thread.h>
#include <core/services/shell/taskbar.h>
#include <core/services/shell/winmgr.h>

namespace os
{
    namespace services
    {
        class shell_host
        {
            public:
                window_manager       winmgr;
                std::gfx::image      framebuffer;
                threading::thread_t* thread;
                shell_taskbar*       taskbar;
                
            private:
                uint32_t _fps;

            public:
                void init();
                void dispose();
                void start();

            public:
                void update();
                void draw();

            private:
                static int main(int argc, char** argv);
            
            public:
                uint32_t fps();
        };
    }
}