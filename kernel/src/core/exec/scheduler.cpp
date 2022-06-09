#include <core/exec/scheduler.h>
#include <core/kernel.h>

namespace os
{
    namespace threading
    {
        uint32_t                   scheduler::_tid;
        std::arraylist<thread_t*> scheduler::_threads;
        uint32_t                   scheduler::_index;
        bool                       scheduler::_ready;
        uint32_t                   scheduler::_now;
        uint32_t                   scheduler::_last;

        void scheduler::init()
        {
            _now   = 0;
            _last  = 0;
            _tid   = 0;
            _index = 0;
            _ready = false;

            kernel::thread      = thread_create_k();
            scheduler::load(kernel::thread);

            THREAD      = kernel::thread;
            THREAD_NEXT = kernel::thread;

            for (size_t i = 0; i < kernel::heap_large.count_max(); i++)
            {
                if (kernel::heap_large.blocks()[i].thread == 0 && kernel::heap_large.blocks()[i].type > ALLOCTYPE_FREE && kernel::heap_large.blocks()[i].size > 0)
                { kernel::heap_large.blocks()[i].thread = kernel::thread; }
            }

            for (size_t i = 0; i < kernel::heap_small.count_max(); i++)
            {
                if (kernel::heap_small.blocks()[i].thread == 0 && kernel::heap_small.blocks()[i].type > ALLOCTYPE_FREE && kernel::heap_small.blocks()[i].size > 0)
                { kernel::heap_small.blocks()[i].thread = kernel::thread; }
            }

            printf("%s Initialized scheduler\n", DEBUG_OK);
        }

        void scheduler::ready() { _ready = true; }

        void scheduler::unready() { _ready = false; }

        void scheduler::yield()
        {
            if (!_ready) { return; }
            if (_threads.length() == 0) { return; }
            asm volatile("cli");
            _ready = false;

            THREAD = _threads[_index];
            if (THREAD->flags.locked) { asm volatile("sti"); _ready = true; return; }
            if (THREAD == NULL) { perror("Current thread null"); return; }

            THREAD_NEXT = next();

            _ready = true;
            _context_switch();
        }

        void scheduler::monitor()
        {
            _now = std::timenow().second;
            if (_now != _last)
            {
                _last = _now;
                uint32_t total_tps = 0;

                for (size_t i = 0; i < _threads.length(); i++)
                {
                    if (_threads[i] == NULL) { continue; }
                    _threads[i]->time.tps = _threads[i]->time.ticks;
                    _threads[i]->time.tpsf = _threads[i]->time.tps / 10;
                    _threads[i]->time.ticks = 0;
                    total_tps += _threads[i]->time.tpsf;
                }

                for (size_t i = 0; i < _threads.length(); i++)
                {
                    if (_threads[i] == NULL) { continue; }
                    if (_threads[i]->time.tpsf == 0) { continue; }
                    double usage = (double)((double)_threads[i]->time.tpsf / (double)total_tps) * 100.0;
                    _threads[i]->time.cpu_usage = (uint8_t)usage;
                }
            }
        }

        bool scheduler::start(thread_t* thread)
        {
            bool ints = hal::idt::irqs_enabled();
            if (ints) { asm volatile("cli"); }
            if (thread == NULL) { return false; }
            thread->flags.running    = true;
            thread->flags.terminated = false;
            thread->flags.locked     = false;
            printf("%s Started thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            if (ints) { asm volatile("sti"); }
            return true;
        }

        bool scheduler::pause(thread_t* thread)
        {
            bool ints = hal::idt::irqs_enabled();
            if (ints) { asm volatile("cli"); }
            thread->flags.running    = false;
            thread->flags.terminated = false;
            thread->flags.locked     = false;
            printf("%s Paused thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            if (ints) { asm volatile("sti"); }
            return true;
        }

        bool scheduler::terminate(thread_t* thread)
        {
            bool ints = hal::idt::irqs_enabled();
            if (ints) { asm volatile("cli"); }
            thread->flags.running    = false;
            thread->flags.terminated = true;
            thread->flags.locked     = false;
            printf("%s Terminated thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            if (ints) { asm volatile("sti"); }
            return true;
        }

        bool scheduler::load(thread_t* thread)
        {
            bool ints = hal::idt::irqs_enabled();
            if (ints) { asm volatile("cli"); }
            if (thread == NULL) { perror("Tried to load null thread"); return false; }
            thread->flags.running    = false;
            thread->flags.locked     = false;
            thread->flags.terminated = false;
            _threads.add(thread);
            printf("%s Loaded thread %u('%s') into scheduler\n", DEBUG_INFO, thread->id, thread->name);
            if (ints) { asm volatile("sti"); }
            return true;
        }

        bool scheduler::unload(thread_t* thread)
        {
            bool ints = hal::idt::irqs_enabled();
            if (ints) { asm volatile("cli"); }
            if (thread == NULL) { perror("Tried to unload null thread"); return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == thread)
                {
                    thread_t* t = _threads[i];
                    _threads.remove_at(i);
                    printf("%s Unloaded thread %u('%s') from scheduler\n", DEBUG_INFO, t->id, t->name);
                    thread_dispose(t);
                    if (ints) { asm volatile("sti"); }
                    return true;
                }
            }

            perror("Failed to unload thread 0x%8x from scheduler", thread);
            return false;
        }

        thread_t* scheduler::next()
        {
            while (true)
            {
                _index++;
                if (_index >= _threads.length()) { _index = 0; return _threads[0]; }
                if (_threads[_index] == NULL) { perror("Next thread null"); }
                if (_threads[_index]->flags.terminated)
                {
                    printf("GONNA DISPOSE THREAD\n");
                    unload(_threads[_index]);
                    _index = 0;
                    return _threads[0];
                }
                if (_threads[_index]->flags.sleeping) { continue; }
                if (_threads[_index]->flags.running)  { return _threads[_index]; }
            }
            return NULL;
        }

        uint32_t scheduler::generate_id()
        {
            uint32_t id = _tid++;
            return id;
        }

        bool scheduler::is_ready() { return _ready; }

        std::arraylist<thread_t*>* scheduler::threads() { return &_threads; }

        bool scheduler::is_running(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == NULL) { continue; }
                if (_threads[i] == thread) { return true; }
            }
            return false;
        }

        bool scheduler::is_running(uint32_t id)
        {
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == NULL) { continue; }
                if (_threads[i]->id == id) { return true; }
            }
            return false;
        }

        bool scheduler::is_running(char* name)
        {
            if (name == NULL || strlen(name) == 0) { return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == NULL) { continue; }
                if (!strcmp(_threads[i]->name, name)) { return true; }
            }
            return false;
        }
    }
}