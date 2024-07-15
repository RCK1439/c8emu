/**
 * @file   ram.c
 * @brief  Implementation of the Chip-8 memory layout and context.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#include "ram.h"
#include "constants.h"
#include "debug.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

/* --- function prototypes ------------------------------------------------- */

/**
 * Loads the Chip-8 fontset to the correct memory region in ram.
 */
static void LoadFont(void);

/* --- global variables ---------------------------------------------------- */

static uint8_t s_Memory[MEMORY_SIZE];

/* --- ram interface ------------------------------------------------------- */

MemoryStatus InitRAM(const char *rom_file)
{
    memset(s_Memory, 0x00, sizeof(s_Memory));
    
    FILE *rom;
    if ((rom = fopen(rom_file, "rb")) == NULL)
        return MEM_FILE_ERR;

    fseek(rom, 0, SEEK_END);
    const size_t size = (size_t)ftell(rom);
    fseek(rom, 0, SEEK_SET);

    uint8_t *buffer;
    if ((buffer = (uint8_t*)malloc(size * sizeof(uint8_t))) == NULL)
    {
        fclose(rom);
        return MEM_ALLOC_ERR;
    }

    fread(buffer, sizeof(uint8_t), size, rom);
    fclose(rom);

    DISASSEMBLE(rom_file, buffer, size);

    memcpy(s_Memory + 0x0200, buffer, size * sizeof(uint8_t));
    free(buffer);

    LoadFont();

    return MEM_OK;
}

void RAMWrite(uint16_t addr, uint8_t val)
{
    addr &= 0x0FFF;
    s_Memory[addr] = val;
}

uint8_t RAMRead(uint16_t addr)
{
    addr &= 0x0FFF;
    return s_Memory[addr];
}

/* --- utility functions --------------------------------------------------- */

static void LoadFont(void)
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
        s_Memory[ADDR_FONT + i] = fontset[i];
}
