#include <common.h>
#include <core/kernel.h>

EXTC
{
    void __cxa_pure_virtual()
    {
        // Do nothing or print an error message.
    }
}

void lock()
{
    if (THREAD == NULL) { return; }
    THREAD->flags.locked = true;
}

void unlock()
{
    if (THREAD == NULL) { return; }
    THREAD->flags.locked = false;
}

void yield()
{
    os::threading::scheduler::yield();
}

void sleep(uint32_t ms)
{
    if (!os::threading::scheduler::is_ready())
    {
        uint32_t now = 0, last = 0, timer = 0;
        while (true)
        {
            now = std::timenow().millis;
            if (now != last) { last = now; timer++; }
            if (timer >= ms) { timer = 0; return; }
        }
    }
}