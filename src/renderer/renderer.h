#ifndef RENDERER_H
#define RENDERER_H

#include "core/types.h"

typedef struct C8Renderer C8Renderer;

C8Renderer *c8InitRenderer(void);
void c8CloseRenderer(C8Renderer *renderer);

void c8RendererBegin(C8Renderer *renderer);
void c8RendererEnd(C8Renderer *renderer);

void c8RendererOnResize(C8Renderer *renderer);

void c8DrawBuffer(C8Renderer *renderer, const u8 *buffer, size_t width, size_t height);

void c8ToggleDebugOverlay(C8Renderer *renderer);
C8Bool c8DebugOverlayEnabled(const C8Renderer *renderer);

void c8AddDebugText(C8Renderer *renderer, const char *fmt, ...);

#endif /* RENDERER_H */

