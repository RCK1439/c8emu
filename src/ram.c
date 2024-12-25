#include "ram.h"
#include "constants.h"
#include "debug.h"
#include "util.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

static void LoadChip8Font(void);

static uint8_t memory[MEMORY_SIZE];

MemoryResult InitRAM(const char *romFile)
{
    memset(memory, 0x00, sizeof(memory));
    
    FILE *rom;
    if ((rom = fopen(romFile, "rb")) == NULL)
    {
        return MEM_FILE_ERR;
    }

    fseek(rom, 0, SEEK_END);
    const size_t size = (size_t)ftell(rom);
    fseek(rom, 0, SEEK_SET);

    uint8_t *buffer = C8_MALLOC(uint8_t, size);
    fread(buffer, sizeof(uint8_t), size, rom);
    fclose(rom);

    DISASSEMBLE(romFile, buffer, size);

    memcpy(memory + 0x0200, buffer, size * sizeof(uint8_t));
    C8_FREE(buffer);

    LoadChip8Font();

    return MEM_OK;
}

void RAMWrite(uint16_t addr, uint8_t val)
{
    addr &= 0x0FFF;
    memory[addr] = val;
}

uint8_t RAMRead(uint16_t addr)
{
    addr &= 0x0FFF;
    return memory[addr];
}

static void LoadChip8Font(void)
{
    const uint8_t fontset[FONTSET_SIZE] = {
    	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    	0x20, 0x60, 0x20, 0x20, 0x70, // 1
    	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (uint8_t i = 0; i < FONTSET_SIZE; i++)
    {
        memory[ADDR_FONT + i] = fontset[i];
    }
}

