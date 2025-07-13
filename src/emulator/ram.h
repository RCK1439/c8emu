#ifndef RAM_H
#define RAM_H

#include "rom.h"
#include "specifications.h"

typedef struct C8RAM
{
    u8 memory[C8_MEMORY_SIZE];
} C8RAM;

C8RAM c8InitRAM(void);

void c8UploadROMToRAM(C8RAM *ram, C8ROM rom);

void c8RAMWrite(C8RAM *ram, u16 addr, u8 val);
u8 c8RAMRead(const C8RAM *ram, u16 addr);

#endif /* RAM_H */
