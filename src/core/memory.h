#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define C8_MALLOC(T, N) (T *)c8Malloc(sizeof(T) * (N))
#define C8_FREE(P) c8Free((P))

void *c8Malloc(size_t numBytes);
void c8Free(void *ptr);

#endif /* MEMORY_H */
