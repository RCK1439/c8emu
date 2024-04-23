#include "emu.h"
#include "ram.h"
#include "cpu.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

#define SCALE WINDOW_WIDTH / 64

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
    InitWindow(1024, 512, "c8emu");
    InitAudioDevice();
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        cpu_step();

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
    size_t x, y;

    for (y = 0; y < 64; y++) {
        for (x = 0; x < 32; x++) {
            if (ram_read_u8(0x0F00 + (x + y * 64))) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }
}