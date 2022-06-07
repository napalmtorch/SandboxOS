#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <core/memory/memmgr.h>

namespace os
{
    typedef struct
    {
        uint32_t  address;
        void*     thread;
        size_t    size;
        ALLOCTYPE type;
    } PACKED heapblock_t;

    class memory_heap
    {   
        friend class garbage_collector;

        private:
            heapblock_t* _blocks;
            size_t       _count;
            size_t       _count_max;
            size_t       _size;
            uint32_t     _alignment;
            uint32_t     _data_start;
            bool         _messages;

        public:
            static void init(bool messages);
            void init(size_t size, size_t count, uint32_t align, bool msgs = true);

        public:
            void*    allocate(size_t size, ALLOCTYPE type, bool clear = false);
            bool     free(void* ptr);
            uint32_t merge();
            void     print();
        
        public:
            heapblock_t* create(uint32_t addr, void* thread, size_t size, ALLOCTYPE type);
            bool         remove(heapblock_t* blk);

        public:
            heapblock_t* next(size_t size);
            heapblock_t* next_empty();
            heapblock_t* nearest(heapblock_t* blk);
            heapblock_t* fetch(void* ptr);
            bool         validate(uint32_t addr);
            bool         validate(void* ptr);

        public:
            void   toggle_msgs(bool state);
            size_t calc_used();
            static const char* typestr(ALLOCTYPE type);

        public:
            uint32_t total_size();
            uint32_t data_size();
            uint32_t table_size();
            uint32_t alignment();
            uint32_t count();
            uint32_t count_max();
            heapblock_t* blocks();
    };
}