#include <core/memory/heap.h>
#include <core/kernel.h>

namespace os
{
    void memory_heap::init(size_t size, size_t count, uint32_t align, bool msgs)
    {
        uint32_t req_size = memalign(size + (count * sizeof(heapblock_t)), 0x1000);
        memblk_t* blk = memory_manager::request(req_size, memblk_type::heap);
        if (blk == NULL) { perror("Failed to initialize heap at 0x%8x", this); }

        this->_size       = size;
        this->_count_max  = count;
        this->_alignment  = align;
        this->_count      = 0;
        this->_data_start = blk->address + (count * sizeof(heapblock_t));
        this->_blocks     = (heapblock_t*)blk->address;
        this->_messages   = msgs;

        create(this->_data_start, NULL, this->_size, ALLOCTYPE_FREE);

        printf("%s Initialized heap(0x%8x) - ADDR:0x%8x ALIGN:0x%8x MAX:%u SIZE:%u MB\n", DEBUG_OK, this, blk->address, align, count, size / MB);
    }

    void* memory_heap::allocate(size_t size, ALLOCTYPE type, bool clear)
    {
        if (size == 0) { perror("Tried to allocate 0 bytes"); }
        heapblock_t* blk = next(memalign(size, this->_alignment));
        if (blk == NULL) { perror("Failed to allocate %u bytes", size); }
        if (clear) { memset((void*)blk->address, 0, blk->size); }
        blk->type   = (blk->type == ALLOCTYPE_INVALID ? ALLOCTYPE_OBJ : type);
        if (_messages) { printf("%s ADDR:0x%8x THREAD:0x%8x TYPE:%s SIZE:%u bytes\n", DEBUG_MALLOC, blk->address, blk->thread, typestr(blk->type), blk->size); }
        return (void*)blk->address;
    }

    bool memory_heap::free(void* ptr)
    {
        if (ptr == NULL) { return false; }
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].address == (uint32_t)ptr && this->_blocks[i].type > ALLOCTYPE_FREE)
            {
                ALLOCTYPE type = this->_blocks[i].type;
                this->_blocks[i].type = ALLOCTYPE_FREE;
                if (_messages) { printf("%s ADDR:0x%8x THREAD:0x%8x TYPE:%s SIZE:%u bytes\n", DEBUG_FREE, this->_blocks[i].address, this->_blocks[i].thread, typestr(type), this->_blocks[i].size); }
                return true;
            }
        }
        perror("Failed to free pointer at 0x%8x", ptr);
        return false;
    }

    uint32_t memory_heap::merge()
    {
        heapblock_t* nblk = NULL;
        size_t c = 0;

        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type != ALLOCTYPE_FREE) { continue; }
            nblk = nearest(&this->_blocks[i]);
            if (nblk != NULL)
            {
                if (this->_blocks[i].address > nblk->address) { this->_blocks[i].address = nblk->address; }
                this->_blocks[i].size += nblk->size;
                remove(nblk);
                c++;
            }
        }

        if (this->_blocks[0].type == ALLOCTYPE_FREE)
        {
            nblk = nearest(&this->_blocks[0]);
            if (nblk != NULL)
            {
                if (this->_blocks[0].address > nblk->address) { this->_blocks[0].address = nblk->address; }
                this->_blocks[0].size += nblk->size;
                remove(nblk);
                c++;
            }
        }
        return c;
    }

    void memory_heap::print()
    {
        printf("HEAP AT 0x%8x\n{\n", this);
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type == ALLOCTYPE_INVALID) { continue; }
            printf("  ADDR:0x%8x THREAD:0x%8x TYPE:%s SIZE:%u bytes\n", this->_blocks[i].address, this->_blocks[i].thread, typestr(this->_blocks[i].type), this->_blocks[i].size);
        }
        printf("}\n\n");
    }

    heapblock_t* memory_heap::create(uint32_t addr, void* thread, size_t size, ALLOCTYPE type)
    {
        if (!validate(addr)) { perror("Attempt to create heap block with invalid address 0x%8x", addr); }
        heapblock_t* blk = next_empty();
        if (blk == NULL) { perror("Failed to create new heap block"); }
        blk->address = addr;
        blk->thread  = thread;
        blk->size    = size;
        blk->type    = type;
        this->_count++;
        return blk;
    }

    bool memory_heap::remove(heapblock_t* blk)
    {
        if (blk == NULL) { perror("Tried to remove null block from heap"); }
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type == ALLOCTYPE_INVALID) { continue; }
            if (&this->_blocks[i] == blk)
            {
                this->_blocks[i] = (heapblock_t){ .address = 0, .thread = NULL, .size = 0, .type = ALLOCTYPE_INVALID };
                this->_count--;
                return true;
            }
        }
        perror("Failed to delete heap block at 0x%8x", blk);
        return false;
    }

    heapblock_t* memory_heap::next(size_t size)
    {
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].size >= size && this->_blocks[i].type == ALLOCTYPE_FREE)
            {
                heapblock_t* blk = create((uint32_t)this->_blocks[i].address, NULL, size, ALLOCTYPE_OBJ);
                this->_blocks[i].address += size;
                this->_blocks[i].size    -= size;
                return blk;
            }
        }
        return NULL;
    }

    heapblock_t* memory_heap::next_empty()
    {
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type == ALLOCTYPE_INVALID) { return &this->_blocks[i]; }
        }
        return NULL;
    }

    heapblock_t* memory_heap::nearest(heapblock_t* blk)
    {
        if (blk == NULL) { return NULL; }
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type != ALLOCTYPE_FREE) { continue; }
            if (this->_blocks[i].address + this->_blocks[i].size == blk->address) { return &this->_blocks[i]; }
            if (blk->address - blk->size == this->_blocks[i].address)             { return &this->_blocks[i]; }
        }
        return NULL;
    }

    heapblock_t* memory_heap::fetch(void* ptr)
    {
        if (ptr == NULL) { return NULL; }
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].type <= ALLOCTYPE_FREE) { continue; }
            if (this->_blocks[i].address == (uint32_t)ptr) { return &this->_blocks[i]; }
        }
        return NULL;
    }

    bool memory_heap::validate(uint32_t addr)
    {
        if (addr < this->_data_start || addr >= this->_data_start + this->_size) { return false; }
        return true;
    }

    bool memory_heap::validate(void* ptr) { return validate((uint32_t)ptr); }

    void memory_heap::toggle_msgs(bool state) { _messages = state; }

    size_t memory_heap::calc_used()
    {
        size_t bytes = 0;
        for (size_t i = 0; i < this->_count_max; i++)
        {
            if (this->_blocks[i].size > 0 && this->_blocks[i].type > ALLOCTYPE_FREE) { bytes += this->_blocks[i].size; }
        }
        return bytes;
    }

    uint32_t memory_heap::total_size() { return data_size() + table_size(); }

    uint32_t memory_heap::data_size() { return this->_size; }

    uint32_t memory_heap::table_size() { return this->_count_max * sizeof(heapblock_t); }
    
    uint32_t memory_heap::alignment() { return this->_alignment; }
    
    uint32_t memory_heap::count() { return this->_count; }

    uint32_t memory_heap::count_max() { return this->_count_max; }

    heapblock_t* memory_heap::blocks() { return this->_blocks; }

    const char* memory_heap::typestr(ALLOCTYPE type)
    {
        switch (type)
        {
            default:               { return "invalid"; }
            case ALLOCTYPE_FREE:   { return "free   "; }
            case ALLOCTYPE_OBJ:    { return "object "; }
            case ALLOCTYPE_STRING: { return "string "; }
            case ALLOCTYPE_ARRAY:  { return "array  "; }
        }
    }
}