#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void c8Panic(Chip8ErrorCode code, const char *fmt, ...)
{
    va_list args = { 0 };

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    
    fputc('\n', stderr);
    exit(code);
}
