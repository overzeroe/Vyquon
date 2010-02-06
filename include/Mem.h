#ifndef MEM_H
#define MEM_H

/* Allocate memory for the VM */
void* VyMalloc(size_t size);
void* VyRealloc(void*, size_t size);

/* Free memory allocated */
void VyFree(void* ptr);

#endif /* MEM_H */
