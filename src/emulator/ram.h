#ifndef RAM_H
#define RAM_H

#include "rom.h"

#define ADDR_SCREEN 0x0F00
#define ADDR_STACK  0x0FA0
#define ADDR_PC     0x0200
#define ADDR_FONT   0x0050

typedef struct Chip8RAM Chip8RAM;

Chip8RAM *c8InitRAM(void);
void c8CloseRAM(Chip8RAM *ram);

void c8UploadROMToRAM(Chip8RAM *ram, Chip8ROM rom);

void c8RAMWrite(Chip8RAM *ram, u16 addr, u8 val);
u8 c8RAMRead(const Chip8RAM *ram, u16 addr);

#endif /* RAM_H */
