#include "renderer.h"
#include "debug_overlay.h"

#include "core/debug.h"
#include "core/memory.h"

#include <raylib.h>
#include <raymath.h>

struct Chip8Renderer
{
    Font               font;
    RenderTexture2D    target;
    Chip8DebugOverlay *debugOverlay;
    Chip8Bool          drawDebugOverlay;
};

static void c8DrawDebugOverlay(Chip8Renderer *renderer);

Chip8Renderer *c8InitRenderer(void)
{
    C8_ASSERT(IsWindowReady(), "Window not yet ready");

    const i32 screenWidth = GetScreenWidth();
    const i32 screenHeight = GetScreenHeight();

    Chip8Renderer *const renderer = C8_MALLOC(Chip8Renderer, 1);
    renderer->font = GetFontDefault();
    renderer->target = LoadRenderTexture(screenWidth, screenHeight);
    renderer->debugOverlay = c8CreateDebugOverlay();
    renderer->drawDebugOverlay = C8_FALSE;
    if (renderer->target.id <= 0)
    {
        c8Panic(ERR_FAILED_TO_LOAD_TARGET, "Failed to load render target");
    }

    return renderer;
}

void c8CloseRenderer(Chip8Renderer *renderer)
{
    c8FreeDebugOverlay(renderer->debugOverlay);
    UnloadRenderTexture(renderer->target);
    C8_FREE(renderer);
}

void c8RendererBegin(Chip8Renderer *renderer)
{
    BeginTextureMode(renderer->target);
    ClearBackground(BLACK);
}

void c8RendererEnd(Chip8Renderer *renderer)
{
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    const Rectangle src = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)renderer->target.texture.width,
        .height = (float)(-renderer->target.texture.height)
    };
    const Rectangle dest = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)renderer->target.texture.width,
        .height = (float)renderer->target.texture.height
    };
    const Vector2 origin = Vector2Zero();
    DrawTexturePro(renderer->target.texture, src, dest, origin, 0.0f, WHITE);
    c8DrawDebugOverlay(renderer);

    EndDrawing();
}

void c8RendererOnResize(Chip8Renderer *renderer)
{
    UnloadRenderTexture(renderer->target);

    const i32 screenWidth = GetScreenWidth();
    const i32 screenHeight = GetScreenHeight();
    renderer->target = LoadRenderTexture(screenWidth, screenHeight);

    C8_LOG_WARNING("Framebuffer resized: %dx%d", screenWidth, screenHeight);
}

void c8DrawBuffer(Chip8Renderer *renderer, const u8 *buffer, size_t width, size_t height)
{
    const float scale = (float)renderer->target.texture.width / (float)width;

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const size_t idx = x + y * width;
            if (!buffer[idx])
            {
                continue;
            }

            const Rectangle pixel = {
                .x = (float)x * scale,
                .y = (float)y * scale,
                .width = scale,
                .height = scale
            };
            DrawRectangleRec(pixel, GREEN);
        }
    }
}

void c8ToggleDebugOverlay(Chip8Renderer *renderer)
{
    renderer->drawDebugOverlay = !renderer->drawDebugOverlay;
}

Chip8Bool c8DebugOverlayEnabled(const Chip8Renderer *renderer)
{
    return renderer->drawDebugOverlay;
}

void c8AddDebugText(Chip8Renderer *renderer, const char *fmt, ...)
{
    if (!renderer->drawDebugOverlay)
    {
        return;
    }

    va_list args = { 0 };

    va_start(args, fmt);
    c8AppendDebugText(renderer->debugOverlay, fmt, args);
    va_end(args);
}

static void c8DrawDebugOverlay(Chip8Renderer *renderer)
{
    const size_t size = c8GetDebugOverlaySize(renderer->debugOverlay);
    
    for (size_t i = 0; i < size; i++)
    {
        const Chip8DebugText debugText = c8GetDebugTextAt(renderer->debugOverlay, i);
        const Vector2 size = MeasureTextEx(renderer->font, debugText.text, 20.0f, 2.0f);
        const Rectangle box = {
            .x = debugText.position.x,
            .y = debugText.position.y,
            .width = size.x + 2.5f,
            .height = size.y + 2.5f,
        };
        const Color boxColor = {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 128
        };

        DrawRectangleRec(box, boxColor);
        DrawTextEx(renderer->font, debugText.text, debugText.position, 20.0f, 2.0f, WHITE);
    }

    c8ClearDebugOverlay(renderer->debugOverlay);
}

