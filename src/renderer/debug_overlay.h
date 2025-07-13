#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H

#include <raylib.h>

#include <stdarg.h>
#include <stddef.h>

#define MAX_DEBUG_TEXT_SIZE (128)

typedef struct C8DebugText
{
    char    text[MAX_DEBUG_TEXT_SIZE+1];
    Vector2 position;
} C8DebugText;

typedef struct C8DebugOverlay C8DebugOverlay;

C8DebugOverlay *c8CreateDebugOverlay(void);
void c8FreeDebugOverlay(C8DebugOverlay *debugOverlay);

void c8ClearDebugOverlay(C8DebugOverlay *debugOverlay);

void c8AppendDebugText(C8DebugOverlay *debugOverlay, const char *fmt, va_list args);

size_t c8GetDebugOverlaySize(const C8DebugOverlay *debugOverlay);
C8DebugText c8GetDebugTextAt(const C8DebugOverlay *debugOverlay, size_t i);

#endif /* DEBUG_OVERLAY_H */

