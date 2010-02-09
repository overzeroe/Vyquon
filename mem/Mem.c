#include "Vyquon.h"

/* List of areas of contiguous memory which are free */
typedef struct _FreeList {
    int size;
    void* mem; 
    struct _FreeList* next; /* NULL if end of heap */
} FreeList;

/* The heap */
FreeList* heap = NULL;

/* Allocate initial heap and such */
int INIT_HEAP_SIZE = 1024 /* 1024 bytes */ * 1024 /* 1024 kilobytes */ * 10 /* 10 megabytes */;
void InitHeap(){
    void* memory = malloc(INIT_HEAP_SIZE);
    heap = malloc(sizeof(FreeList));
    heap->mem = memory;
    heap->size = INIT_HEAP_SIZE;
    heap->next = NULL;
}

/* Find the next available piece of memory on the heap of at least the given size
 * return the pointer to it, and also return the previous piece of memory in prev_ptr
 */
FreeList* FindNextFreeMem(size_t size, FreeList** prev_ptr){
    *prev_ptr = NULL;
    FreeList* ptr = heap;
    while(ptr->size < size){
        *prev_ptr = ptr;
        ptr = ptr->next;

        if(ptr == NULL)
            break;
    }

    return ptr;
}

void* VyMalloc(size_t size){
    if(!heap) InitHeap();
    
    /* Find free memory */
    FreeList* prev;
    FreeList* next_available = FindNextFreeMem(size, &prev);
    void* allocated = next_available->mem;

    /* Mark it as unfree */
    next_available->size -= size;
    next_available->mem += size;

    /* It no longer exists if it's empty */
    if(prev && next_available->size <= 0){
        prev->next = next_available->next;
        free(next_available);
    }

    return allocated;
}

void* VyNoHeapMalloc(size_t size){
    return malloc(size);
}
void* VyNoHeapRealloc(void* ptr, size_t new_size){
    return realloc(ptr, new_size);
}
void VyNoHeapFree(void* ptr){
    free(ptr);
}
