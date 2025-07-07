#include "client.h"

#include "core/memory.h"
#include "core/platform.h"

#include "emulator/chip8.h"

#include <raylib.h>
#include <stdio.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 512

#define REFRESH_RATE 60

struct Chip8Client
{
    Chip8    *emulator;
    Chip8Bool isRunning;
    Chip8Bool debug;
};

static void c8ClientOnUpdate(Chip8Client *client);
static void c8ClientOnRender(const Chip8Client *client);

Chip8Client *c8InitClient(i32 argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return NULL;
    }

    Chip8Client *const client = C8_MALLOC(Chip8Client, 1);
    client->emulator = c8InitEmulator();
    client->isRunning = C8_TRUE;
    client->debug = C8_FALSE;

#if !defined(C8_DEBUG)
    SetTraceLogLevel(LOG_NONE);
#endif
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "c8emu");
    InitAudioDevice();
    SetTargetFPS(REFRESH_RATE);

    c8LoadROMInEmulator(client->emulator, argv[1]);
    return client;
}

void c8RunClient(Chip8Client *client)
{
    while (client->isRunning)
    {
        c8ClientOnUpdate(client);
        c8ClientOnRender(client);
    }
}

void c8CloseClient(Chip8Client *client)
{
    CloseAudioDevice();
    CloseWindow();

    c8CloseEmulator(client->emulator);
    C8_FREE(client);
}

static void c8ClientOnUpdate(Chip8Client *client)
{
    c8EmulatorOnUpdate(client->emulator);

    client->isRunning = !WindowShouldClose();
}

static void c8ClientOnRender(const Chip8Client *client)
{
    BeginDrawing();
    ClearBackground(BLACK);
        c8EmulatorOnRender(client->emulator);
    EndDrawing();
}
