#include <common.h>
#include <core/kernel.h>

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