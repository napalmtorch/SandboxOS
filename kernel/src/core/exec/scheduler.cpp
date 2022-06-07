#include <core/exec/scheduler.h>
#include <core/kernel.h>

EXTC
{
    extern void _context_switch();
}

namespace os
{
    namespace threading
    {
        uint32_t                   scheduler::sleeptime;
        uint32_t                   scheduler::ticks;
        uint32_t                   scheduler::_tid;
        std::arraylist<thread_t*> scheduler::_threads;
        uint32_t                   scheduler::_index;
        bool                       scheduler::_ready;

        /// @brief Initialize thread scheduler
        void scheduler::init()
        {
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

        /// @brief Notify scheduler that its allowed to context switch
        void scheduler::ready() { _ready = true; }

        /// @brief Switch to next available thread in list 
        OPTIMIZE("O0") void scheduler::yield()
        {
            if (!_ready) { return; }
            if (_threads.length() == 0) { return; }
            asm volatile("cli");
            _ready = false;

            THREAD = _threads[_index];
            if (THREAD->flags.locked) { asm volatile("sti"); _ready = true; return; }
            if (THREAD == NULL) { perror("Current thread null"); return; }

            THREAD_NEXT = next();
            if (THREAD_NEXT == NULL) { perror("Next thread null"); return; }

            _ready = true;
            _context_switch();
        }

        /// @brief Start specified thread @param thread Pointer to thread @return Thread started successfully
        bool scheduler::start(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            thread->flags.running    = true;
            thread->flags.terminated = false;
            thread->flags.locked     = false;
            printf("%s Started thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            return true;
        }

        /// @brief Pause specified thread @param thread Pointer to thread @return Thread paused successfully
        bool scheduler::pause(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            thread->flags.running    = false;
            thread->flags.terminated = false;
            thread->flags.locked     = false;
            printf("%s Paused thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            return true;
        }

        /// @brief Send a request to terminate specified thread @param thread Pointer to thread @return Thread termination request sent successfully
        bool scheduler::terminate(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            thread->flags.running    = false;
            thread->flags.terminated = true;
            thread->flags.locked     = false;
            printf("%s Terminated thread %u('%s')\n", DEBUG_INFO, thread->id, thread->name);
            return true;
        }

        /// @brief Load specified thread into scheduler queue @param thread Pointer to thread @return Thread loaded successfully
        bool scheduler::load(thread_t* thread)
        {
            if (thread == NULL) { perror("Tried to load null thread"); return false; }
            thread->flags.running    = false;
            thread->flags.locked     = false;
            thread->flags.terminated = false;
            _threads.add(thread);
            printf("%s Loaded thread %u('%s') into scheduler\n", DEBUG_INFO, thread->id, thread->name);
            return true;
        }

        /// @brief Unload specified thread into scheduler queue @param thread Pointer to thread @return Thread unloaded successfully
        bool scheduler::unload(thread_t* thread)
        {
            if (thread == NULL) { perror("Tried to unload null thread"); return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == thread)
                {
                    thread_t* t = _threads[i];
                    _threads.remove_at(i);
                    printf("%s Unloaded thread %u('%s') from scheduler\n", DEBUG_INFO, t->id, t->name);
                    thread_dispose(t);
                    return true;
                }
            }

            perror("Failed to unload thread 0x%8x from scheduler", thread);
            return false;
        }

        /// @brief Fetch next available thread in queue @return Pointer to thread
        thread_t* scheduler::next()
        {
            while (true)
            {
                _index++;
                if (_index >= _threads.length()) { _index = 0; return _threads[0]; }
                if (_threads[_index]->flags.terminated)
                {
                    unload(_threads[_index]);
                    _index = 0;
                    return _threads[0];
                }
                if (_threads[_index]->flags.sleeping) { continue; }
                if (_threads[_index]->flags.running)  { return _threads[_index]; }
            }
            return NULL;
        }

        /// @brief Generate unused ID @return ID value
        uint32_t scheduler::generate_id()
        {
            uint32_t id = _tid++;
            return id;
        }

        /// @brief Check if scheduler is allowed to context switch @return Can switch
        bool scheduler::is_ready() { return _ready; }

        /// @brief Get list of threads @return Pointer to thread list
        std::arraylist<thread_t*>* scheduler::threads() { return &_threads; }

        /// @brief Check if specified thread is running @param thread Pointer to thread @return Thread is running
        bool scheduler::is_running(thread_t* thread)
        {
            if (thread == NULL) { return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i] == thread) { return true; }
            }
            return false;
        }

        /// @brief Check if specified thread is running @param id ID of thread @return Thread is running
        bool scheduler::is_running(uint32_t id)
        {
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (_threads[i]->id == id) { return true; }
            }
            return false;
        }

        /// @brief Check if specified thread is running @param name Name of thread @return Thread is running
        bool scheduler::is_running(char* name)
        {
            if (name == NULL || strlen(name) == 0) { return false; }
            for (size_t i = 0; i < _threads.length(); i++)
            {
                if (!strcmp(_threads[i]->name, name)) { return true; }
            }
            return false;
        }
    }
}