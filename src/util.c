#include "util.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void *CheckedMalloc(size_t numBytes)
{
    void *const p = malloc(numBytes);
    if (!p)
    {
        Panic("Out of memory!");
    }

    return p;
}

void Free(void *p)
{
    free(p);
}

void Panic(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    putchar('\n');
    exit(3);
}
