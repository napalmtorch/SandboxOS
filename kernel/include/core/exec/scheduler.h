#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <arraylist.h>
#include <core/exec/thread.h>

EXTC
{
    /// @brief Switch thread contexts - only meant to be performed by os::threading::scheduler::yield
    extern void _context_switch(void);
}

namespace os
{
    namespace threading
    {
        /// @brief Thread scheduler service - Used for queuing and manipulating threads
        class scheduler
        {
            /// @internal Allow access to private members
            friend class thread;

            private:
                /// @brief Last generated thread id - increments by one each generation
                static uint32_t _tid;
                /// @brief Dynamic array list for queuing threads
                static std::arraylist<thread_t*> _threads;
                /// @brief Current thread index
                static uint32_t _index;
                /// @brief Toggle whether scheduler is allowed to context switch
                static bool _ready;

            public:
                /// @brief Initialize thread scheduler
                static void init();

                /// @brief Allow scheduler to start context switching
                static void ready();

                /// @brief Stop scheduler from context switching
                static void unready();

                /// @brief Perform context switch into the next thread in the queue
                static void yield();

                /// @brief Start specified thread @param thread Pointer to thread @return Thread started successfully
                static bool start(thread_t* thread);

                /// @brief Pause specified thread @param thread Pointer to thread @return Thread paused successfully
                static bool pause(thread_t* thread);

                /// @brief Send request to terminate specified thread @param thread Pointer to thread @return Thread termination request sent successfully
                static bool terminate(thread_t* thread);

                /// @brief Load specified thread into queue @param thread Pointer to thread @return Thread loaded successfully
                static bool load(thread_t* thread);

                /// @brief Unload specified thread from queue if it exists @param thread Pointer to thread @return Thread unloaded successfully
                static bool unload(thread_t* thread);

                /// @brief Generate new thread ID and increment @return Thread identification number
                static uint32_t generate_id();

                /// @brief Check is scheduler is allowed to context switch @return Context switching enabled
                static bool is_ready();

                /// @brief Get thread queue list @return Pointer to thread queue
                static std::arraylist<thread_t*>* threads();
                

            public:
                /// @brief Check if specified thread is running @param thread Pointer to thread @return Thread is running
                static bool is_running(thread_t* thread);

                /// @brief Check if specified thread is running @param thread Thread ID @return Thread is running
                static bool is_running(uint32_t id);

                /// @brief Check if specified thread is running @param thread Pointer to thread name @return Thread is running
                static bool is_running(char* name);

            private:
                /// @brief Get next available thread in scheduler @return Pointer to thread
                static thread_t* next();
        };
    }
}