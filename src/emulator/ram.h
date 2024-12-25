#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

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

