#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <gfx/image.h>
#include <core/exec/thread.h>

namespace os
{
    namespace services
    {
        class shell_host
        {
            public:
                std::gfx::image      framebuffer;
                threading::thread_t* thread;

                
            private:
                uint32_t _fps;

            public:
                void init();
                void dispose();
                void start();

            private:
                static int main(int argc, char** argv);
            
            public:
                uint32_t fps();
        };
    }
}