#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define C8_MALLOC(T, N)     (T *)c8Malloc(sizeof(T) * (N))
#define C8_REALLOC(T, P, N) (T *)c8Realloc((P), sizeof(T) * (N))
#define C8_FREE(P)          c8Free((P))

void *c8Malloc(size_t numBytes);
void *c8Realloc(void *ptr, size_t numBytes);
void c8Free(void *ptr);

#endif /* MEMORY_H */
