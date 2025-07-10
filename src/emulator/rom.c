#include "rom.h"
#include "specifications.h"

#include "core/debug.h"
#include "core/memory.h"

#include <memory.h>
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
    if (rom.size > C8_MAX_ROM_SIZE)
    {
        C8_LOG_ERROR("ROM file too large!");
        return rom;
    }
    fseek(file, 0, SEEK_SET);

    rom.data = C8_MALLOC(u8, rom.size);
    fread(rom.data, sizeof(u8), rom.size, file);
    fclose(file);
    
    rom.romName = romFile;

    C8_LOG_INFO("ROM successfully loaded "SIZE_T_FMT" bytes: %s", rom.size, romFile);
    return rom;
}

void c8UnloadROM(Chip8ROM rom)
{
    C8_FREE(rom.data);
    C8_LOG_WARNING("ROM unloaded");
}
