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

/* --- type definitions ---------------------------------------------------- */

/**
 * This struct defines the context for the Chip-8 memory.
 */
typedef struct ram_context_s {
    uint8_t ram[MEMORY_SIZE];
} ram_context_t;

/* --- function prototypes ------------------------------------------------- */

/**
 * Loads the Chip-8 fontset to the correct memory region in ram.
 */
static void load_font(void);

/* --- global variables ---------------------------------------------------- */

static ram_context_t ctx;

/* --- ram interface ------------------------------------------------------- */

ram_status_t ram_init(const char *rom_file) {
    memset(ctx.ram, 0x00, sizeof(ctx.ram));
    
    FILE *rom;
    if (fopen_s(&rom, rom_file, "rb") != 0) {
        return RAM_FILE_ERR;
    }

    fseek(rom, 0, SEEK_END);
    const size_t size = (size_t)ftell(rom);
    fseek(rom, 0, SEEK_SET);

    uint8_t *buffer;
    if ((buffer = (uint8_t*)malloc(size * sizeof(uint8_t))) == NULL) {
        fclose(rom);
        return RAM_ALLOC_ERR;
    }

    fread(buffer, sizeof(uint8_t), size, rom);
    fclose(rom);

    DISASSEMBLE(rom_file, buffer, size);

    memcpy(ctx.ram + 0x0200, buffer, size * sizeof(uint8_t));
    free(buffer);

    load_font();

    return RAM_OK;
}

void ram_write(uint16_t addr, uint8_t val) {
    addr &= 0x0FFF;
    ctx.ram[addr] = val;
}

uint8_t ram_read(uint16_t addr) {
    addr &= 0x0FFF;
    return ctx.ram[addr];
}

/* --- utility functions --------------------------------------------------- */

static void load_font(void) {
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

    for (uint8_t i = 0; i < FONTSET_SIZE; i++) {
        ctx.ram[ADDR_FONT + i] = fontset[i];
    }
}
