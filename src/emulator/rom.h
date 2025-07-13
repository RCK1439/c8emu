#ifndef ROM_H
#define ROM_H

#include "core/types.h"

typedef struct C8ROM
{
    u8         *data;
    size_t      size;
    const char *romName;
} C8ROM;

C8ROM c8LoadROM(const char *romFile);
void c8UnloadROM(C8ROM rom);

#endif /* ROM_H */
