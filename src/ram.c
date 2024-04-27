#include "ram.h"
#include "constants.h"
#include "debug.h"
#include "instructions.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ram_context_s {
    uint8_t ram[MEMORY_SIZE];
} ram_context_t;

static void load_font(void);

static ram_context_t ctx;

ram_status_t ram_init(const char *rom_file)
{
    FILE *rom;
    size_t size;
    uint8_t *buffer;

    memset(ctx.ram, 0x00, sizeof(ctx.ram));
    if ((rom = fopen(rom_file, "rb")) == NULL) {
        return RAM_FILE_ERR;
    }

    fseek(rom, 0, SEEK_END);
    size = (size_t)ftell(rom);
    fseek(rom, 0, SEEK_SET);

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

void ram_write(uint16_t addr, uint8_t val)
{
    addr &= 0x0FFF;
    ctx.ram[addr] = val;
}

uint8_t ram_read(uint16_t addr)
{    
    addr &= 0x0FFF;
    return ctx.ram[addr];
}

static void load_font(void)
{
    uint8_t i, fontset[FONTSET_SIZE] = {
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

    for (i = 0; i < FONTSET_SIZE; i++) {
        ctx.ram[ADDR_FONT + i] = fontset[i];
    }
}
