#include "emu.h"
#include "ram.h"

#include <stdio.h>

emu_status_t emu_run(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return EMU_USAGE_ERR;
    }

    switch (ram_init(argv[1])) {
        case RAM_FILE_ERR: {
            fprintf(stderr, "couldn't load rom: %s\n", argv[1]);
        } return EMU_RAM_ERR;
        case RAM_ALLOC_ERR: {
            fprintf(stderr, "couldn't allocate rom buffer\n");
        } return EMU_RAM_ERR;
        default: break;
    }

    return EMU_OK;
}