#ifndef ROM_H
#define ROM_H

#include "core/types.h"

#define MAX_ROM_SIZE ((4 * 1024) - 512)

typedef struct Chip8ROM
{
    u8     data[MAX_ROM_SIZE];
    size_t size;
} Chip8ROM;

Chip8ROM c8LoadROM(const char *romFile);
void c8UnloadROM(Chip8ROM rom);

#endif /* ROM_H */
