#include "memory.h"
#include "debug.h"
#include "platform.h"

#include <stdlib.h>

// --- memory interface -------------------------------------------------------

void *c8Malloc(size_t numBytes)
{
    void *const ptr = malloc(numBytes);
    if (!ptr)
    {
        c8Panic(ERR_OUT_OF_MEMORY, "Out of memory");
    }

    C8_LOG_WARNING("allocated "SIZE_T_FMT" bytes at ptr=%p", numBytes, ptr);
    return ptr;
}

void *c8Realloc(void *ptr, size_t numBytes)
{
    void *const p = realloc(ptr, numBytes);
    if (!p)
    {
        c8Panic(ERR_OUT_OF_MEMORY, "Out of memory");
    }

    C8_LOG_WARNING("reallocated "SIZE_T_FMT" bytes to ptr=%p", numBytes, p);
    return p;
}

void c8Free(void *ptr)
{
    C8_LOG_WARNING("freed ptr=%p", ptr);
    free(ptr);
}
