#include "debug_overlay.h"

#include "core/debug.h"
#include "core/memory.h"

#include <stdio.h>

#define INIT_CAPACITY (16)
#define INIT_POSITION (Vector2) { 5.0f, 5.0f }

struct Chip8DebugOverlay
{
    Chip8DebugText *debugTexts;
    size_t          size;
    size_t          capacity;
    Vector2         currPosition;
};

static void c8DebugOverlayEnsureSpace(Chip8DebugOverlay *debugOverlay);

Chip8DebugOverlay *c8CreateDebugOverlay(void)
{
    Chip8DebugOverlay *const debugOverlay = C8_MALLOC(Chip8DebugOverlay, 1);
    debugOverlay->debugTexts = C8_MALLOC(Chip8DebugText, INIT_CAPACITY);
    debugOverlay->size = 0;
    debugOverlay->capacity = INIT_CAPACITY;
    debugOverlay->currPosition = INIT_POSITION;

    return debugOverlay;
}

void c8FreeDebugOverlay(Chip8DebugOverlay *debugOverlay)
{
    C8_FREE(debugOverlay->debugTexts);
    C8_FREE(debugOverlay);
}

void c8ClearDebugOverlay(Chip8DebugOverlay *debugOverlay)
{
    debugOverlay->size = 0;
    debugOverlay->currPosition = INIT_POSITION;
}

void c8AppendDebugText(Chip8DebugOverlay *debugOverlay, const char *fmt, va_list args)
{
    c8DebugOverlayEnsureSpace(debugOverlay);

    Chip8DebugText debugText = { 0 };

    vsnprintf(debugText.text, MAX_DEBUG_TEXT_SIZE, fmt, args);
    debugText.position = debugOverlay->currPosition;

    debugOverlay->currPosition.y += 20.0f + 5.0f; // font size + spacing
    debugOverlay->debugTexts[debugOverlay->size++] = debugText;
}

size_t c8GetDebugOverlaySize(const Chip8DebugOverlay *debugOverlay)
{
    return debugOverlay->size;
}

Chip8DebugText c8GetDebugTextAt(const Chip8DebugOverlay *debugOverlay, size_t i)
{
    C8_ASSERT(i < debugOverlay->size, "Index, "SIZE_T_FMT", out of range for size: "SIZE_T_FMT, i, debugOverlay->size);
    return debugOverlay->debugTexts[i];
}

static void c8DebugOverlayEnsureSpace(Chip8DebugOverlay *debugOverlay)
{
    if (debugOverlay->size < debugOverlay->capacity)
    {
        return;
    }

    debugOverlay->capacity = 3 * debugOverlay->capacity / 2;
    debugOverlay->debugTexts = C8_REALLOC(Chip8DebugText, debugOverlay->debugTexts, debugOverlay->capacity);
}

