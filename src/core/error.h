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
    ERR_NONE,
    ERR_ASSERTION_FAILED,
    ERR_FAILED_TO_OPEN_FILE,
    ERR_FAILED_TO_READ_ROM,
    ERR_OUT_OF_MEMORY,
    ERR_FAILED_TO_LOAD_TARGET,
} Chip8ErrorCode;

void c8Panic(Chip8ErrorCode code, const char *fmt, ...);

#endif /* ERROR_H */
