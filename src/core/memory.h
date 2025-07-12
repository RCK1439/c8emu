#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

// --- useful memory macros ---------------------------------------------------

#define C8_MALLOC(T, N)     (T *)c8Malloc(sizeof(T) * (N))
#define C8_REALLOC(T, P, N) (T *)c8Realloc((P), sizeof(T) * (N))
#define C8_FREE(P)          c8Free((P))

// --- memory interface -------------------------------------------------------

/**
 * Attempts to allocate `numBytes` bytes of memory
 *
 * @param[in] numBytes
 *      The number of bytes of memory to allocate
 *
 * @return
 *      A pointer to the start of the allocated memory
 */
void *c8Malloc(size_t numBytes);

/**
 * Attempts to reallocate `numBytes` bytes of memory at the address, `ptr`
 *
 * @param[in] ptr
 *      A pointer to the memory to reallocate
 * @param[in] numBytes
 *      The new number of bytes to reallocate the memory with
 *
 * @return
 *      A pointer to the start of the reallocated memory
 */
void *c8Realloc(void *ptr, size_t numBytes);

/**
 * Deallocates the memory allocated at `ptr`
 *
 * @param[in] ptr
 *      A pointer to the memory to deallocate
 */
void c8Free(void *ptr);

#endif /* MEMORY_H */
