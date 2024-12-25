#include "emu.h"
#include "constants.h"
#include "cpu.h"
#include "ram.h"

#include <raylib.h>
#include <stdio.h>

static void ProcessInput(void);

EmulatorResult RunEmulator(int32_t argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return EMU_USAGE_ERR;
    }

    switch (InitRAM(argv[1]))
    {
        case MEM_FILE_ERR:
        {
            fprintf(stderr, "couldn't load rom: %s\n", argv[1]);
        } return EMU_RAM_ERR;
        case MEM_ALLOC_ERR:
        {
            fprintf(stderr, "couldn't allocate rom buffer\n");
        } return EMU_RAM_ERR;
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

    return EMU_OK;
}

static void ProcessInput(void)
{
    const KeyboardKey keyboard_keys[NUM_KEYS] = {
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
        if (IsKeyDown(keyboard_keys[k]))
        {
            SetCPUKey(k, 1);
        }
        else
        {
            SetCPUKey(k, 0);
        }
    }
}
