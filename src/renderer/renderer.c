#include "renderer.h"
#include "debug_overlay.h"

#include "core/debug.h"
#include "core/memory.h"
#include "core/platform.h"

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
    float            scale;            // Scale factor for rendering
};

// --- utility functions ------------------------------------------------------

/**
 * Draws the debug overlay to the window
 * 
 * @param[in] renderer
 *      A handle to the renderer
 */
static void c8DrawDebugOverlay(const C8Renderer *renderer);

// --- renderer implementation ------------------------------------------------

C8Renderer *c8InitRenderer(size_t width, size_t height)
{
    C8_ASSERT(IsWindowReady(), "Window not yet ready");

    C8Renderer *const renderer = C8_MALLOC(C8Renderer, 1);
    renderer->font = GetFontDefault();
    renderer->target = LoadRenderTexture((i32)width, (i32)height);
    if (renderer->target.id == 0)
    {
        c8Panic(ERR_FAILED_TO_LOAD_TARGET, "Failed to load render target");
    }
    
    renderer->debugOverlay = c8CreateDebugOverlay();
    renderer->drawDebugOverlay = C8_FALSE;
    renderer->scale = (float)GetScreenWidth() / (float)width;

    return renderer;
}

void c8CloseRenderer(C8Renderer *renderer)
{
    c8FreeDebugOverlay(renderer->debugOverlay);
    UnloadRenderTexture(renderer->target);
    C8_FREE(renderer);
}

void c8RendererBegin(const C8Renderer *renderer)
{
    BeginTextureMode(renderer->target);
    ClearBackground(C8_BG_COLOR);
}

void c8RendererEnd(const C8Renderer *renderer)
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
        .y = ((float)GetScreenHeight() - (float)renderer->target.texture.height * renderer->scale) * 0.5f,
        .width = (float)renderer->target.texture.width * renderer->scale,
        .height = (float)renderer->target.texture.height * renderer->scale
    };
    const Vector2 origin = Vector2Zero();
    DrawTexturePro(renderer->target.texture, src, dest, origin, 0.0f, WHITE);
    c8DrawDebugOverlay(renderer);

    EndDrawing();
}

void c8RendererOnResize(C8Renderer *renderer)
{
    renderer->scale = (float)GetScreenWidth() / (float)renderer->target.texture.width;
    C8_LOG_WARNING("Framebuffer scale adjusted: %.2f", renderer->scale);
}

void c8DrawBuffer(UNUSED const C8Renderer *renderer, const u8 *buffer, size_t width, size_t height)
{
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const size_t idx = x + y * width;
            if (!buffer[idx])
            {
                continue;
            }

            DrawPixel((i32)x, (i32)y, C8_FG_COLOR);
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

static void c8DrawDebugOverlay(const C8Renderer *renderer)
{
    const size_t size = c8GetDebugOverlaySize(renderer->debugOverlay);
    
    for (size_t i = 0; i < size; i++)
    {
        const C8DebugText debugText = c8GetDebugTextAt(renderer->debugOverlay, i);
        const Vector2 size = MeasureTextEx(renderer->font, debugText.text, 20.0f, 2.0f);
        const Rectangle box = {
            .x = debugText.position.x - 5.0f,
            .y = debugText.position.y - 5.0f,
            .width = size.x + 10.0f,
            .height = size.y + 5.0f,
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

