#include "memory.h"
#include "error.h"
#include "log.h"
#include "platform.h"

#include <stdlib.h>

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

void c8Free(void *ptr)
{
    C8_LOG_WARNING("freed ptr=%p", ptr);
    free(ptr);
}
