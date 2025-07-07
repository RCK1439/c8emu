#include "memory.h"
#include "error.h"

#include <stdlib.h>

void *c8Malloc(size_t numBytes)
{
    void *const ptr = malloc(numBytes);
    if (!ptr)
    {
        c8Panic(ERR_OUT_OF_MEMORY, "Out of memory");
    }

    return ptr;
}

void c8Free(void *ptr)
{
    free(ptr);
}
