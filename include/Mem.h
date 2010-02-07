#ifndef MEM_H
#define MEM_H

/* Memory is complicated. At the moment, we just have functions which
 * allocate memory as needed using stdlibs malloc(). They're used pervasively.
 * Basically this means we lose lots of memory. We need a GC. We might eventually
 * split this into two types of functions: one for auxillary memory, such as that used
 * by the tokenizer and whatever, and one for object data on the heap. One would be
 * garbage collected, and the other would be manually managed. (In fact, we could
 * provide an interface for the manual memory managed part to the language runtime,
 * thus allowing the user to bypass garbage collection if desired.
 */

/* Allocate memory for the VM */
void* VyMalloc(size_t size);
void* VyRealloc(void*, size_t size);

/* Free memory allocated */
void VyFree(void* ptr);

#endif /* MEM_H */
