#include "Vyquon.h"

/* All memory functions are at the moment just wrappers around stdlib's malloc, realloc, and free */
void* VyMalloc(size_t size){
    return malloc(size);
}
void* VyRealloc(void* ptr, size_t size){
    return realloc(ptr, size);
}
void VyFree(void* ptr){
    free(ptr);
}
