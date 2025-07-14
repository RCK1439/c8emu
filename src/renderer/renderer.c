#include "renderer.h"
#include "debug_overlay.h"

#include "core/debug.h"
#include "core/memory.h"

#include <raylib.h>
#include <raymath.h>

// --- some color stuff -------------------------------------------------------

#define RL_COLOR(R, G, B)\
    CLITERAL(Color) {    \
        .r = (u8)(R),    \
        .g = (u8)(G),    \
        .b = (u8)(B),    \
        .a = (u8)255     \
    }                    \

// Background color
#define C8_BG_COLOR RL_COLOR(0, 0, 255)
// Foreground color
#define C8_FG_COLOR RL_COLOR(255, 255, 255)

// --- types ------------------------------------------------------------------

/**
 * Implementation of renderering type handle
 */
struct C8Renderer
{
    Font             font;             // Text font
    RenderTexture2D  target;           // Render target
    C8DebugOverlay  *debugOverlay;     // Debug overlay handle
    C8Bool           drawDebugOverlay; // Flag for whether overlay is enabled
};

// --- utility functions ------------------------------------------------------

/**
 * Draws the debug overlay to the window
 * 
 * @param[in] renderer
 *      A handle to the renderer
 */
static void c8DrawDebugOverlay(C8Renderer *renderer);

// --- renderer implementation ------------------------------------------------

C8Renderer *c8InitRenderer(void)
{
    C8_ASSERT(IsWindowReady(), "Window not yet ready");

    const i32 screenWidth = GetScreenWidth();
    const i32 screenHeight = screenWidth / 2;

    C8Renderer *const renderer = C8_MALLOC(C8Renderer, 1);
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

void c8CloseRenderer(C8Renderer *renderer)
{
    c8FreeDebugOverlay(renderer->debugOverlay);
    UnloadRenderTexture(renderer->target);
    C8_FREE(renderer);
}

void c8RendererBegin(C8Renderer *renderer)
{
    BeginTextureMode(renderer->target);
    ClearBackground(C8_BG_COLOR);
}

void c8RendererEnd(C8Renderer *renderer)
{
    EndTextureMode();

    BeginDrawing();
    ClearBackground(C8_BG_COLOR);

    const Rectangle src = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)renderer->target.texture.width,
        .height = (float)(-renderer->target.texture.height)
    };
    const Rectangle dest = {
        .x = 0.0f,
        .y = (float)(GetScreenHeight() - renderer->target.texture.height) * 0.5f,
        .width = (float)renderer->target.texture.width,
        .height = (float)renderer->target.texture.height
    };
    const Vector2 origin = Vector2Zero();
    DrawTexturePro(renderer->target.texture, src, dest, origin, 0.0f, WHITE);
    c8DrawDebugOverlay(renderer);

    EndDrawing();
}

void c8RendererOnResize(C8Renderer *renderer)
{
    UnloadRenderTexture(renderer->target);

    const i32 screenWidth = GetScreenWidth();
    const i32 screenHeight = screenWidth / 2;
    renderer->target = LoadRenderTexture(screenWidth, screenHeight);
    if (renderer->target.id <= 0)
    {
        C8_LOG_ERROR("Failed to resize render target");
    }

    C8_LOG_WARNING("Framebuffer resized: %dx%d", screenWidth, screenHeight);
}

void c8DrawBuffer(C8Renderer *renderer, const u8 *buffer, size_t width, size_t height)
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
            DrawRectangleRec(pixel, C8_FG_COLOR);
        }
    }
}

void c8ToggleDebugOverlay(C8Renderer *renderer)
{
    renderer->drawDebugOverlay = !renderer->drawDebugOverlay;
}

C8Bool c8DebugOverlayEnabled(const C8Renderer *renderer)
{
    return renderer->drawDebugOverlay;
}

void c8AddDebugText(C8Renderer *renderer, const char *fmt, ...)
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

// --- utility function implementation ----------------------------------------

static void c8DrawDebugOverlay(C8Renderer *renderer)
{
    const size_t size = c8GetDebugOverlaySize(renderer->debugOverlay);
    
    for (size_t i = 0; i < size; i++)
    {
        const C8DebugText debugText = c8GetDebugTextAt(renderer->debugOverlay, i);
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

