#include "Vyquon.h"

/* List of areas of contiguous memory which are free */
typedef struct _FreeList {
    int size;
    void* mem; 
    struct _FreeList* next; /* NULL if end of heap */
} FreeList;

/* The heap */
FreeList* heap = NULL;

/* Pointers to the start of the heap and the end. 
 * The pointer to the end points to the first byte that isn't in the heap, so 
 * under no circumstances should you dereference it!
 */
void* heap_start, heap_end;

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

void* VyMalloc(VyType type){
    if(!heap) InitHeap();
    
    /* Find free memory */
    FreeList* prev;
    FreeList* next_available = FindNextFreeMem(size + sizeof(VyType) + sizeof(char), &prev);
    void* allocated = next_available->mem;

    /* Allocate a byte for the mark */
    char* mark = allocated;
    allocated += sizeof(char);
    *mark = current_mark ? 0 : 1;
    
    /* Store the size of the allocated memory right before it */
    VyType* store_size = allocated;
    allocated += sizeof(VyType);
    *store_size = type;

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

/* Allocate memory for bytecode specifically */
GPtrArray* bytecode_arr = NULL;
void* VyBytecodeInstrAlloc(size_t size){
    if(!bytecode_arr)
        bytecode_arr = g_ptr_array_new();

    void* mem = VyNoHeapMalloc(size);
    g_ptr_array_add(bytecode_arr, mem);
    return mem;
}
void* VyBytecodeInstrFree(Instruction* instrs){
    g_ptr_array_remove_fast(instrs);
    VyNoHeapFree(instrs);
}

/* Scan the system stack to check for pointers to objects on the heap */
void* __stack_start, __stack_end;
void ScanSystemStack(){
    void** stack_start = STACK_START();
    void** stack_end = STACK_END();
    void** cur_ptr = stack_start;

    while(cur_ptr <= stack_end){
        if(IsInHeap(*cur_ptr)){
            VyType type = *((VyType*) cur_ptr - sizeof(VyType));
            GCMark(WrapObj(*cur_ptr, type));
        }
        cur_ptr += sizeof(char);
    }
}

/* Scan the interpreter stack to check for pointers to objects on the heap */
void ScanInterpreterStack(){
    MarkAllOnStack();
}

/* Find pointers to objects on the heap inside bytecodes */
void MarkBytecode(void* bcode, void* nothing){
    Bytecode* instrs = bcode;
    int size = instrs->used;
    int i;
    for(i = 0; i < size; i++){
        Instruction instr = instrs->instructions[i];
        if(instr.opcode == INSTR_PUSH){
            GCMark(instr.data.obj);
        }
    }
}
void ScanBytecodes(){
    g_ptr_array_foreach(bytecode_arr, &MarkBytecode, NULL);
}

/* Mark and sweep garbage collector */
char current_mark = 1;
bool IsMarked(void* ptr){
    ptr -= sizeof(VyType);
    ptr -= sizeof(char);
    return current_mark == *((char*) ptr);
}
void SetMarked(void* ptr){
    ptr -= sizeof(VyType);
    ptr -= sizeof(char);
    *((char*) ptr) = current_mark;
}
/* Check whether a pointer points to a place in the heap */
bool IsInHeap(void* ptr){
    return ptr < heap_end && ptr >= heap_start;
}
void GarbageCollect(){
    /* No more objects will be allocated until GC is over */
    MARK_STACK_END();

    /* Mark all objects that shouldn't be freed */
    /* Roots: system stack, interpreter stack, bytecode */
    ScanSystemStack();
    ScanInterpreterStack();
    ScanBytecodes();

    /* Reverse what we consider to be marked, so that all marked objects become unmarked again */
    current_mark = current_mark ? 0 : 1;
}

void ForceGC(){
    GarbageCollect();
}
void GCMark(VyObj obj){
    SetMarked(Obj(obj));
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
