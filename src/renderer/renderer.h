#ifndef RENDERER_H
#define RENDERER_H

#include "core/types.h"

typedef struct Chip8Renderer Chip8Renderer;

Chip8Renderer *c8InitRenderer(void);
void c8CloseRenderer(Chip8Renderer *renderer);

void c8RendererBegin(Chip8Renderer *renderer);
void c8RendererEnd(Chip8Renderer *renderer);

void c8RendererOnResize(Chip8Renderer *renderer);

void c8DrawBuffer(Chip8Renderer *renderer, const u8 *buffer, size_t width, size_t height);

void c8ToggleDebugOverlay(Chip8Renderer *renderer);
Chip8Bool c8DebugOverlayEnabled(const Chip8Renderer *renderer);

void c8AddDebugText(Chip8Renderer *renderer, const char *fmt, ...);

#endif /* RENDERER_H */

