#include <core/kernel.h>

void *operator new(size_t size) { return tmalloc(size, ALLOCTYPE_OBJ); }
void *operator new[](size_t size) { return tmalloc(size, ALLOCTYPE_ARRAY); }
void operator  delete(void *p) { free(p); }
void operator  delete(void *p, size_t size) { free(p); }
void operator  delete[](void *p) { free(p); }
void operator  delete[](void *p, size_t size) { free(p); }

os::heapblock_t* memfetch(void* ptr)
{
    if (ptr == NULL) { return NULL; }
    os::heapblock_t* blk = os::kernel::heap_small.fetch(ptr);
    if (blk != NULL) { return blk; }
    blk = os::kernel::heap_large.fetch(ptr);
    if (blk != NULL) { return blk; }
    return NULL; 
}
