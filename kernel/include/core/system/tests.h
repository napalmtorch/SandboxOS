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

            /// @brief Test functionality of data streams @param iterations Times to loop @return Test has passed
            bool test_stream(int iterations);

            /// @brief Test functionality of arraylist @param iterations Times to loop @return Test has passed
            bool test_arraylist(int iterations);

            /// @brief Test functionality of scheduler @param iterations Times to loop @return Test has passed
            bool test_scheduler(int iterations);
        }
    }
}