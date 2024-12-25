#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

#define C8_MALLOC(T, N) (T *)CheckedMalloc(sizeof(T) * (N))
#define C8_FREE(P) Free((void *)P)

#define C8_ASSERT(expr, ...) if (!(expr)) Panic(__VA_ARGS__)

void *CheckedMalloc(size_t numBytes);
void Free(void *p);

void Panic(const char *fmt, ...);

#endif /* UTIL_H */
