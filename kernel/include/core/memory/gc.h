#pragma once
#include <stdint.h>
#include <stddef.h>
#include <core/exec/thread.h>

namespace os
{
    class garbage_collector
    {
        friend class memory_heap;

        public:
            static threading::thread_t* thread;
            static bool                 messages;
        
        public:
            static void start();
            static void run();
            static void merge();
            static void collect();
            
        private:
            static int main(int argc, char** argv);
    };
}