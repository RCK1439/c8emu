#include "client.h"
#include "config.h"

#include "core/debug.h"
#include "core/memory.h"
#include "core/platform.h"

#include "emulator/chip8.h"

#include "renderer/renderer.h"

#include <raylib.h>

// --- type definitions -------------------------------------------------------

/**
 * This struct "contains" the process as a whole. The whole emulator is tied to
 * the lifetime of this struct
 */
struct C8Client
{
    C8Emulator *emulator;   // A handle to the emulator
    C8Renderer *renderer;   // A handle to the renderer
    C8Bool      isRunning;  // Flag indicating whether the application is running or not
    float       updateTime; // The time taken, in seconds, to update the logic of the application
    float       renderTime; // The time taken, in seconds, to render the application to the window
};

// --- static functions -------------------------------------------------------

/**
 * Called once per frame to update the logic of the client
 *
 * @param[in] client
 *      The handle to the client
 */
static void c8ClientOnUpdate(C8Client *client);

/**
 * Called once per frame to render the client to the window
 *
 * @param[in] client
 *      The handle to the client
 */
static void c8ClientOnRender(C8Client *client);

/**
 * Called once the window has been resized, to adjust the window content
 *
 * @param[in] client
 *      The handle to the client
 */
static void c8ClientOnResize(C8Client *client);

#if defined(C8_DEBUG)
/**
 * This is simply a callback logging function so that raylib can use the
 * internal logging system that we have got here
 */
static void c8RaylibLogger(int logLevel, const char *fmt, va_list args);
#endif

// --- client interface -------------------------------------------------------

C8Client *c8InitClient(i32 argc, char **argv)
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

    C8Client *const client = C8_MALLOC(C8Client, 1);
    client->emulator = c8InitEmulator();
    client->renderer = c8InitRenderer();
    client->isRunning = C8_TRUE;
    client->updateTime = 0.0f;
    client->renderTime = 0.0f;

    if (argc > 1)
    {
        c8LoadROMInEmulator(client->emulator, argv[1]);
    }

    return client;
}

void c8RunClient(C8Client *client)
{
    while (client->isRunning)
    {
        c8ClientOnUpdate(client);
        c8ClientOnRender(client);
    }
}

void c8CloseClient(C8Client *client)
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

// --- static function implementations ----------------------------------------

static void c8ClientOnUpdate(C8Client *client)
{
    const float t0 = (float)GetTime();
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
    client->updateTime = (float)GetTime() - t0;
}

static void c8ClientOnRender(C8Client *client)
{
    const float t0 = (float)GetTime();

    if (c8DebugOverlayEnabled(client->renderer))
    {
        c8AddDebugText(client->renderer, "Client:");
        c8AddDebugText(client->renderer, "- Resolution: %dx%d", GetScreenWidth(), GetScreenHeight());
        c8AddDebugText(client->renderer, "- %d FPS (%.2fms)", GetFPS(), GetFrameTime() * 1000.0f);
        c8AddDebugText(client->renderer, "- Update time: %.5fms", client->updateTime * 1000.0f);
        c8AddDebugText(client->renderer, "- Render time: %.5fms", client->renderTime * 1000.0f);
    }

    c8RendererBegin(client->renderer); 
        c8EmulatorOnRender(client->emulator, client->renderer);
    c8RendererEnd(client->renderer);

    client->renderTime = (float)GetTime() - t0;
}

static void c8ClientOnResize(C8Client *client)
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

