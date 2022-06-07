#pragma once
#include <stdint.h>
#include <stddef.h>

namespace os
{
    namespace sys
    {
        namespace tests
        {
            /// @brief Allocate and free random chunks of memory for x many iterations @param interations Times to loop @return Test has passed
            bool test_heap(int iterations);
        }
    }
}