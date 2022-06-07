#include <core/system/tests.h>
#include <core/kernel.h>

namespace os
{
    namespace sys
    {
        namespace tests
        {
            bool test_heap(int iterations)
            {
                uint32_t used_before = kernel::heap_large.calc_used() + kernel::heap_small.calc_used();
                uint32_t used_after  = 0;
                for (size_t i = 0; i < iterations; i++)
                {
                    void* test1 = tmalloc(128, ALLOCTYPE_STRING);
                    void* test2 = tmalloc(32 * KB, ALLOCTYPE_ARRAY);
                    void* test3 = tmalloc(256 * KB, ALLOCTYPE_ARRAY);
                    void* test4 = tmalloc(8192, ALLOCTYPE_OBJ);
                    void* test5 = tmalloc(6, ALLOCTYPE_OBJ);
                    void* test6 = tmalloc(0x10000, ALLOCTYPE_STRING);
                    if (!free(test4)) { perror("Failed to free pointer at 0x%8x", test4); }
                    if (!free(test2)) { perror("Failed to free pointer at 0x%8x", test2); }
                    if (!free(test3)) { perror("Failed to free pointer at 0x%8x", test3); }
                    if (!free(test1)) { perror("Failed to free pointer at 0x%8x", test1); }
                    if (!free(test6)) { perror("Failed to free pointer at 0x%8x", test6); }
                    if (!free(test5)) { perror("Failed to free pointer at 0x%8x", test5); }
                }

                uint32_t mc1 = kernel::heap_large.merge();
                uint32_t mc2 = kernel::heap_small.merge();
                if (mc1 + mc2 == 0) { perror("Expected heap merge count to be greater than 0"); return false; }
                else { printf("%s Merged %u free heap entries\n", DEBUG_INFO, mc1 + mc2); }

                if (used_before != used_after) { perror("Unexpected memory leak detected while testing heap"); return false; }
                printf("%s Heap test passed\n", DEBUG_OK);
                return true;
            }
        }
    }
}