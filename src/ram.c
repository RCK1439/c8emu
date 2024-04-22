#include "ram.h"
#include "instructions.h"
#ifndef NDEBUG
#include "debug.h"
#endif

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 4096

typedef struct ram_context_s {
    uint8_t ram[MEMORY_SIZE];
} ram_context_t;

static ram_context_t ctx;

ram_status_t ram_init(const char *rom_file)
{
    FILE *rom;
    size_t size;
    uint8_t *buffer;

    ctx = (ram_context_t) { 0 };
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

#ifndef NDEBUG
    disassemble(rom_file, buffer, size);
#endif

    memcpy(ctx.ram + 0x0200, buffer, size * sizeof(uint8_t));
    free(buffer);

    return RAM_OK;
}

void ram_write(uint16_t addr, uint8_t val)
{
    addr &= 0x0FFF;
    ctx.ram[addr] = val;
}

uint16_t ram_read(uint16_t addr)
{
    addr &= 0x0FFF;
    return ctx.ram[addr];
}
