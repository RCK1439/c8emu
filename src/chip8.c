#include "chip8.h"
#include "constants.h"
#include "cpu.h"
#include "ram.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

#define REFRESH_RATE 60

static void ProcessInput(void);

Chip8ExitCode InvokeChip8(int32_t argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return C8_USAGE_ERR;
    }

    switch (InitRAM(argv[1]))
    {
        case MEM_FILE_ERR:
        {
            fprintf(stderr, "couldn't load rom: %s\n", argv[1]);
        } return C8_RAM_ERR;
        case MEM_ALLOC_ERR:
        {
            fprintf(stderr, "couldn't allocate rom buffer\n");
        } return C8_RAM_ERR;
        default: break;
    }

    InitCPU();

#ifdef NDEBUG
    SetTraceLogLevel(LOG_NONE);
#endif
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "c8emu");
    InitAudioDevice();
    SetTargetFPS(REFRESH_RATE);
    
    while (!WindowShouldClose())
    {
        ProcessInput();
        StepCPU();
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawCPUBuffer();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return C8_OK;
}

static void ProcessInput(void)
{
    const KeyboardKey keys[NUM_KEYS] = {
        KEY_ZERO,
        KEY_ONE,
        KEY_TWO,
        KEY_THREE,
        KEY_FOUR,
        KEY_FIVE,
        KEY_SIX,
        KEY_SEVEN,
        KEY_EIGHT,
        KEY_NINE,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F
    };

    for (uint8_t k = 0; k < NUM_KEYS; k++)
    {
        if (IsKeyDown(keys[k]))
        {
            SetCPUKey(k, 1);
        }
        else
        {
            SetCPUKey(k, 0);
        }
    }
}
