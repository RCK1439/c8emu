#include "client.h"

#include "core/log.h"
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

#if defined(C8_DEBUG)
static void c8RaylibLogger(int logLevel, const char *fmt, va_list args);
#endif

Chip8Client *c8InitClient(i32 argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <rom_file>\n", argv[0]);
        return NULL;
    }

#if defined(C8_RELEASE)
    SetTraceLogLevel(LOG_NONE);
#else
    c8InitLogging();
    SetTraceLogCallback(c8RaylibLogger);

    C8_LOG_INFO("Logging initialized");
#endif

    Chip8Client *const client = C8_MALLOC(Chip8Client, 1);
    client->emulator = c8InitEmulator();
    client->isRunning = C8_TRUE;
    client->debug = C8_FALSE;

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

#if defined(C8_DEBUG)
    c8CloseLogging();
#endif
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

#if defined(C8_DEBUG)
static void c8RaylibLogger(int logLevel, const char *fmt, va_list args)
{
    switch (logLevel)
    {
        case LOG_INFO:
            c8LogInfoArgs(fmt, args);
            break;
        case LOG_WARNING:
            c8LogWarning(fmt, args);
            break;
        case LOG_ERROR:
            c8LogErrorArgs(fmt, args);
            break;
        case LOG_FATAL:
            c8LogFatalArgs(fmt, args);
            break;
    }
}
#endif

