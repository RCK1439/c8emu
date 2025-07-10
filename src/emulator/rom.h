#ifndef ROM_H
#define ROM_H

#include "core/types.h"

typedef struct Chip8ROM
{
    u8         *data;
    size_t      size;
    const char *romName;
} Chip8ROM;

Chip8ROM c8LoadROM(const char *romFile);
void c8UnloadROM(Chip8ROM rom);

#endif /* ROM_H */
