#ifndef ERROR_H
#define ERROR_H

#include "platform.h"

#if defined(C8_DEBUG)
#   define C8_ASSERT(expr, ...) if (!(expr)) c8Panic(ERR_ASSERTION_FAILED, __VA_ARGS__)
#else
#   define C8_ASSERT(expr, ...) (void)0
#endif

typedef enum Chip8ErrorCode
{
    ERR_ASSERTION_FAILED = 1,
    ERR_FAILED_TO_OPEN_FILE = 2,
    ERR_FAILED_TO_READ_ROM = 3,
    ERR_OUT_OF_MEMORY = 4,
} Chip8ErrorCode;

void c8Panic(Chip8ErrorCode code, const char *fmt, ...);

#endif /* ERROR_H */
