#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <core/exec/thread.h>

namespace os
{
    namespace sys
    {
        typedef struct
        {
            char* name;
            char* help;
            char* usage;
        } PACKED command_t;

        class command_handler
        {
            public:
                static threading::thread_t* thread;

            public:
                static void init();
                static void push(char* input);

            private:
                static int main(int argc, char** argv);
        };
    }
}