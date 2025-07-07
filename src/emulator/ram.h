#ifndef RAM_H
#define RAM_H

#include "rom.h"

#define ADDR_ROM    0x0200
#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

#define MEMORY_SIZE (4 * 1024)

typedef struct Chip8RAM
{
    u8 memory[MEMORY_SIZE];
} Chip8RAM;

Chip8RAM c8InitRAM(void);

void c8UploadROMToRAM(Chip8RAM *ram, Chip8ROM rom);

void c8RAMWrite(Chip8RAM *ram, u16 addr, u8 val);
u8 c8RAMRead(const Chip8RAM *ram, u16 addr);

#endif /* RAM_H */
