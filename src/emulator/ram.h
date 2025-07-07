#ifndef RAM_H
#define RAM_H

#include "rom.h"
#include "specifications.h"

typedef struct Chip8RAM
{
    u8 memory[C8_MEMORY_SIZE];
} Chip8RAM;

Chip8RAM c8InitRAM(void);

void c8UploadROMToRAM(Chip8RAM *ram, Chip8ROM rom);

void c8RAMWrite(Chip8RAM *ram, u16 addr, u8 val);
u8 c8RAMRead(const Chip8RAM *ram, u16 addr);

#endif /* RAM_H */
