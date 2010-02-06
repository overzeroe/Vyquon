#include "Vyquon.h"

void* VyMalloc(size_t size){
    return malloc(size);
}

void* VyRealloc(void* ptr, size_t size){
    return realloc(ptr, size);
}

void VyFree(void* ptr){
    free(ptr);
}
