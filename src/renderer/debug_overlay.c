#include "debug_overlay.h"

#include "core/debug.h"
#include "core/memory.h"

#include <stdio.h>

// --- constants --------------------------------------------------------------

#define INIT_CAPACITY (16)
#define INIT_POSITION (Vector2) { 5.0f, 5.0f }

// --- type definition --------------------------------------------------------

/**
 * This is the actual "implementation" of the debug overlay type
 */
struct C8DebugOverlay
{
    C8DebugText *buffer;       // The container for the debug text elements
    size_t       size;         // The current number of elements in the buffer
    size_t       capacity;     // The current capacity of the buffer
    Vector2      nextPosition; // The next position on screen for incoming text
};

// --- utility functions ------------------------------------------------------

static void c8DebugOverlayEnsureSpace(C8DebugOverlay *debugOverlay);

// --- debug overlay implementation -------------------------------------------

C8DebugOverlay *c8CreateDebugOverlay(void)
{
    C8DebugOverlay *const debugOverlay = C8_MALLOC(C8DebugOverlay, 1);
    debugOverlay->buffer = C8_MALLOC(C8DebugText, INIT_CAPACITY);
    debugOverlay->size = 0;
    debugOverlay->capacity = INIT_CAPACITY;
    debugOverlay->nextPosition = INIT_POSITION;

    return debugOverlay;
}

void c8FreeDebugOverlay(C8DebugOverlay *debugOverlay)
{
    C8_FREE(debugOverlay->buffer);
    C8_FREE(debugOverlay);
}

void c8ClearDebugOverlay(C8DebugOverlay *debugOverlay)
{
    debugOverlay->size = 0;
    debugOverlay->nextPosition = INIT_POSITION;
}

void c8AppendDebugText(C8DebugOverlay *debugOverlay, const char *fmt, va_list args)
{
    c8DebugOverlayEnsureSpace(debugOverlay);

    C8DebugText debugText = { 0 };

    vsnprintf(debugText.text, MAX_DEBUG_TEXT_SIZE, fmt, args);
    debugText.position = debugOverlay->nextPosition;

    debugOverlay->nextPosition.y += 20.0f + 5.0f; // font size + spacing
    debugOverlay->buffer[debugOverlay->size++] = debugText;
}

size_t c8GetDebugOverlaySize(const C8DebugOverlay *debugOverlay)
{
    return debugOverlay->size;
}

C8DebugText c8GetDebugTextAt(const C8DebugOverlay *debugOverlay, size_t i)
{
    C8_ASSERT(i < debugOverlay->size, "Index, %zu, out of range for size: %zu", i, debugOverlay->size);
    return debugOverlay->buffer[i];
}

// --- utility function implementations ---------------------------------------

static void c8DebugOverlayEnsureSpace(C8DebugOverlay *debugOverlay)
{
    if (debugOverlay->size < debugOverlay->capacity)
    {
        return;
    }

    debugOverlay->capacity = 3 * debugOverlay->capacity / 2;
    debugOverlay->buffer = C8_REALLOC(C8DebugText, debugOverlay->buffer, debugOverlay->capacity);
}

