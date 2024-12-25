#ifndef RAM_H
#define RAM_H

#include <stdint.h>

typedef enum MemoryResult
{
    MEM_OK,
    MEM_FILE_ERR,
    MEM_ALLOC_ERR
} MemoryResult;

MemoryResult InitRAM(const char *romFile);

void RAMWrite(uint16_t addr, uint8_t val);
uint8_t RAMRead(uint16_t addr);

#endif /* RAM_H */

