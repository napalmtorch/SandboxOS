#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <arraylist.h>
#include <core/exec/thread.h>

namespace os
{
    namespace threading
    {
        class scheduler
        {
            friend class thread;

            public:
                static uint32_t sleeptime;
                static uint32_t ticks;

            private:
                static uint32_t                  _tid;
                static std::arraylist<thread_t*> _threads;
                static uint32_t                  _index;
                static bool                      _ready;

            public:
                static void init();
                static void ready();
                static void yield();
                static bool start(thread_t* thread);
                static bool pause(thread_t* thread);
                static bool terminate(thread_t* thread);
                static bool load(thread_t* thread);
                static bool unload(thread_t* thread);
                static uint32_t generate_id();
                static bool     is_ready();
                static std::arraylist<thread_t*>* threads();
                

            public:
                static bool is_running(thread_t* thread);
                static bool is_running(uint32_t id);
                static bool is_running(char* name);

            private:
                static thread_t* next();
        };
    }
}