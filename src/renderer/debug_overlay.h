#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H

#include <raylib.h>

#include <stdarg.h>
#include <stddef.h>

#define MAX_DEBUG_TEXT_SIZE (128)

typedef struct Chip8DebugText
{
    char    text[MAX_DEBUG_TEXT_SIZE+1];
    Vector2 position;
} Chip8DebugText;

typedef struct Chip8DebugOverlay Chip8DebugOverlay;

Chip8DebugOverlay *c8CreateDebugOverlay(void);
void c8FreeDebugOverlay(Chip8DebugOverlay *debugOverlay);

void c8ClearDebugOverlay(Chip8DebugOverlay *debugOverlay);

void c8AppendDebugText(Chip8DebugOverlay *debugOverlay, const char *fmt, va_list args);

size_t c8GetDebugOverlaySize(const Chip8DebugOverlay *debugOverlay);
Chip8DebugText c8GetDebugTextAt(const Chip8DebugOverlay *debugOverlay, size_t i);

#endif /* DEBUG_OVERLAY_H */

