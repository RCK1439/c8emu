#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>

// --- type definitions -------------------------------------------------------

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef enum C8Bool
{
    C8_FALSE = 0,
    C8_TRUE  = 1,
} C8Bool;

#endif /* TYPES_H */
