#include "ram.h"

#include <memory.h>

// --- utility functions ------------------------------------------------------

/**
 * Loads the Chip-8 font into memory
 *
 * @param[in] ram
 *		A pointer to the Chip-8 RAM
 */
static void c8LoadFont(C8RAM *ram);

// --- ram implementation -----------------------------------------------------

C8RAM c8InitRAM(void)
{
    C8RAM ram = { 0 };

    c8LoadFont(&ram);
    return ram;
}

void c8UploadROMToRAM(C8RAM *ram, C8ROM rom)
{
    memcpy(ram->memory + C8_ADDR_ROM, rom.data, rom.size);
}

void c8RAMWrite(C8RAM *ram, u16 addr, u8 val)
{
    addr &= 0x0FFF;
    ram->memory[addr] = val;
}

u8 c8RAMRead(const C8RAM *ram, u16 addr)
{
    addr &= 0x0FFF;
    return ram->memory[addr];
}

// --- function implementations -----------------------------------------------

static void c8LoadFont(C8RAM *ram)
{
    const u8 fontset[C8_FONTSET_SIZE] = {
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

    for (u8 i = 0; i < C8_FONTSET_SIZE; i++)
    {
        ram->memory[C8_ADDR_FONT + i] = fontset[i];
    }
}

