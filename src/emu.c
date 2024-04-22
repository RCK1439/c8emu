#include "emu.h"
#include "ram.h"

#include <raylib.h>
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

#ifdef NDEBUG
    SetTraceLogLevel(LOG_NONE);
#endif
    InitWindow(1024, 512, "c8emu");
    InitAudioDevice();
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(5, 5);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return EMU_OK;
}