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