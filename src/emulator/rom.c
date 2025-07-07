#include "rom.h"

#include "core/error.h"
#include "core/memory.h"

#include <stdio.h>

Chip8ROM c8LoadROM(const char *romFile)
{
    Chip8ROM rom = { 0 };

    FILE *const file = fopen(romFile, "rb");
    if (!file)
    {
        c8Panic(ERR_FAILED_TO_OPEN_FILE, "Couldn't open file: %s", romFile);
    }

    fseek(file, 0, SEEK_END);
    rom.size = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    rom.data = C8_MALLOC(uint8_t, rom.size);
    fread(rom.data, sizeof(uint8_t), rom.size, file);
    fclose(file);

    return rom;
}

void c8UnloadROM(Chip8ROM rom)
{
    C8_FREE(rom.data);
}
