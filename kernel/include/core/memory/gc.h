#pragma once
#include <stdint.h>
#include <stddef.h>
#include <core/exec/thread.h>

namespace os
{
    /// @brief Garbage collection service - Cleans abandoned allocations and periodically merges free entries
    class garbage_collector
    {
        /// @internal Allow access to private members
        friend class memory_heap;

        public:
            /// @brief Garbage collector thread instance
            static threading::thread_t* thread;
            /// @brief Toggle whether garbage collector prints debugging messages
            static bool messages;
        
        public:
            /// @brief Start garbage collector thread
            static void start();

            /// @brief Merge free entries in kernel heaps
            static void merge();

            /// @brief Collect abandoned allocations from terminated threads
            static void collect();
            
        private:
            /// @brief Garbage collector thread entry point @param argc Argument count @param argv Pointer to arguments @return Exit code - returns 0 if successful
            static int main(int argc, char** argv);
    };
}