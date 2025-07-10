#include "client.h"
#include "config.h"

#include "core/log.h"
#include "core/memory.h"
#include "core/platform.h"

#include "emulator/chip8.h"

#include "renderer/renderer.h"

#include <raylib.h>

struct Chip8Client
{
    Chip8         *emulator;
    Chip8Renderer *renderer;
    Chip8Bool      isRunning;
    Chip8Bool      debug;
};

static void c8ClientOnUpdate(Chip8Client *client);
static void c8ClientOnRender(const Chip8Client *client);
static void c8ClientOnResize(Chip8Client *client);

#if defined(C8_DEBUG)
static void c8RaylibLogger(int logLevel, const char *fmt, va_list args);
#endif

Chip8Client *c8InitClient(i32 argc, char **argv)
{
#if defined(C8_RELEASE)
    SetTraceLogLevel(LOG_NONE);
#else
    c8InitLogging();
    SetTraceLogCallback(c8RaylibLogger);

    C8_LOG_INFO("Logging initialized");
#endif

    if (argc < 2)
    {
        C8_LOG_FATAL("usage: %s <rom_file>", argv[0]);
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(C8_WINDOW_WIDTH, C8_WINDOW_HEIGHT, C8_WINDOW_TITLE);
    InitAudioDevice();

    SetTargetFPS(C8_TARGET_FPS);
    SetWindowMinSize(C8_WINDOW_WIDTH, C8_WINDOW_HEIGHT);

    Chip8Client *const client = C8_MALLOC(Chip8Client, 1);
    client->emulator = c8InitEmulator();
    client->renderer = c8InitRenderer();
    client->isRunning = C8_TRUE;
    client->debug = C8_FALSE;

    if (argc > 1)
    {
        c8LoadROMInEmulator(client->emulator, argv[1]);
    }

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
    c8CloseRenderer(client->renderer);
    c8CloseEmulator(client->emulator);
    
    CloseAudioDevice();
    CloseWindow();

    C8_FREE(client);

#if defined(C8_DEBUG)
    c8CloseLogging();
#endif
}

static void c8ClientOnUpdate(Chip8Client *client)
{
    if (IsKeyPressed(KEY_F3))
    {
        c8ToggleDebugOverlay(client->renderer);
    }

    if (IsKeyPressed(KEY_F11))
    {
        if (!IsWindowFullscreen())
        {
            const i32 monitorHandle = GetCurrentMonitor();
            const i32 monitorWidth = GetMonitorWidth(monitorHandle);
            const i32 monitorHeight = GetMonitorHeight(monitorHandle);
            C8_LOG_WARNING("monitor (%d) resolution: %dx%d", monitorHandle, monitorWidth, monitorHeight);

            SetWindowSize(monitorWidth, monitorHeight);
        }
        
        ToggleFullscreen();
        c8ClientOnResize(client);

        C8_LOG_WARNING("Window resized to %dx%d", GetScreenWidth(), GetScreenHeight());
    }
    else if (IsWindowResized())
    {
        c8ClientOnResize(client);
        C8_LOG_WARNING("Window resized to %dx%d", GetScreenWidth(), GetScreenHeight());
    }

    c8EmulatorOnUpdate(client->emulator);

    client->isRunning = !WindowShouldClose();
}

static void c8ClientOnRender(const Chip8Client *client)
{
    if (c8DebugOverlayEnabled(client->renderer))
    {
        c8AddDebugText(client->renderer, "Client:");
        c8AddDebugText(client->renderer, "- Resolution: %dx%d", GetScreenWidth(), GetScreenHeight());
        c8AddDebugText(client->renderer, "- %d FPS (%.2fms)", GetFPS(), GetFrameTime() * 1000.0f);
    }

    c8RendererBegin(client->renderer); 
        c8EmulatorOnRender(client->emulator, client->renderer);
    c8RendererEnd(client->renderer);
}

static void c8ClientOnResize(Chip8Client *client)
{
    c8RendererOnResize(client->renderer);
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

