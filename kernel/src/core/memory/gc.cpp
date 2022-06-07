#include <core/memory/gc.h>
#include <core/kernel.h>

namespace os
{
    threading::thread_t* garbage_collector::thread;
    bool                 garbage_collector::messages;
        
    void garbage_collector::start()
    {
        messages = false;
        thread   = threading::thread_create("gcollect", 64 * KB, main, 0, NULL);
        threading::scheduler::load(thread);
        threading::scheduler::start(thread);
        printf("%s Initialized garbage collector\n", DEBUG_OK);
    }

    void garbage_collector::run()
    {
        lock();
        collect();
        merge();
        unlock();
        yield();
    }
    
    void garbage_collector::merge()
    {
        uint32_t c = kernel::heap_small.merge() + kernel::heap_large.merge();
        if (c > 0 && messages) { printf("%s Merged %u free heap entries\n", DEBUG_INFO, c); }
    }

    void garbage_collector::collect()
    {
        uint32_t c = 0;

        for (size_t i = 0; i < kernel::heap_large._count_max; i++)
        {
            if (kernel::heap_large._blocks[i].type <= ALLOCTYPE_FREE || kernel::heap_large._blocks[i].size == 0) { continue; }
            if (!threading::scheduler::is_running((threading::thread_t*)kernel::heap_large._blocks[i].thread))
            {
                free((void*)kernel::heap_large._blocks[i].address);
                c++;
            }
        }

        for (size_t i = 0; i < kernel::heap_small._count_max; i++)
        {
            if (kernel::heap_small._blocks[i].type <= ALLOCTYPE_FREE || kernel::heap_small._blocks[i].size == 0) { continue; }
            if (!threading::scheduler::is_running((threading::thread_t*)kernel::heap_small._blocks[i].thread))
            {
                free((void*)kernel::heap_small._blocks[i].address);
                c++;
            }
        }

        if (c > 0) { printf("%s Collected %u abandoned heap allocations\n", DEBUG_INFO, c); }
    }
    
    int garbage_collector::main(int argc, char** argv)
    {
        lock();
        printf("%s Entered garbage collector thread\n", DEBUG_INFO);
        unlock();

        int now = 0, last = 1, timerm = 0, timerc = 0;
        while (true)
        {
            threading::thread_monitor();
            now = hal::pit::millis();
            if (now != last) { last = now; timerm++; timerc++; }
            if (timerm >= 5) { timerm = 0; lock(); merge(); unlock(); yield(); }
            if (timerc >= 50) { timerc = 0; lock(); collect(); unlock(); }
        }
    }
}