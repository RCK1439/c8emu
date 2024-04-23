#include "emu.h"
#include "ram.h"
#include "cpu.h"
#include "constants.h"

#include <raylib.h>
#include <stdio.h>

static void draw_screen_buffer(void);

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

    cpu_init();

#ifdef NDEBUG
    SetTraceLogLevel(LOG_NONE);
#endif
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "c8emu");
    InitAudioDevice();
    SetTargetFPS(REFRESH_RATE);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        draw_screen_buffer();

        DrawFPS(5, 5);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return EMU_OK;
}

static void draw_screen_buffer(void)
{
    size_t x, y, addr;

    for (y = 0; y < 32; y++) {
        for (x = 0; x < 64; x++) {
            addr = 0x0F000 + (x + y * 64);

            if (ram_read_u8(addr)) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }
}