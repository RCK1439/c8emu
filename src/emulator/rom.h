#ifndef ROM_H
#define ROM_H

#include "specifications.h"

#include "core/types.h"

typedef struct Chip8ROM
{
    u8          data[C8_MAX_ROM_SIZE];
    size_t      size;
    const char *romName;
} Chip8ROM;

Chip8ROM c8LoadROM(const char *romFile);
void c8UnloadROM(Chip8ROM rom);

#endif /* ROM_H */
