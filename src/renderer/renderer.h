#ifndef RENDERER_H
#define RENDERER_H

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * Forward decleration of the rendering handle
 */
typedef struct C8Renderer C8Renderer;

// --- renderer interface -----------------------------------------------------

/**
 * Initializes the renderer
 *
 * @param[in] width
 *      The width of the framebuffer
 * @param[in] height
 *      The height of the framebuffer
 *
 * @return
 *      A handle to the renderer
 */
C8Renderer *c8InitRenderer(size_t width, size_t height);

/**
 * Releases any resources used by the renderer and closes the renderer
 *
 * @param[in] renderer
 *      A handle to the renderer
 */
void c8CloseRenderer(C8Renderer *renderer);

/**
 * Instructs the renderer to begin drawing
 *
 * @param[in] renderer
 *      A handle to the renderer
 */
void c8RendererBegin(const C8Renderer *renderer);

/**
 * Instructs the renderer to end drawing
 *
 * @param[in] renderer
 *      A handle to the renderer
 */
void c8RendererEnd(const C8Renderer *renderer);

/**
 * To be called once the window has been resized to adjust the renderer to the
 * new screen dimensions
 *
 * @param[in] renderer
 *      A handle to the renderer
 */
void c8RendererOnResize(C8Renderer *renderer);

/**
 * Draws a `width` by `height` pixel buffer to the window
 *
 * @param[in] renderer
 *      A handle to the renderer
 * @param[in] buffer
 *      The pixel buffer
 * @param[in] width
 *      The width of the pixel buffer
 * @param[in] height
 *      The height of the pixel buffer
 */
void c8DrawBuffer(const C8Renderer *renderer, const u8 *buffer, size_t width, size_t height);

/**
 * Enables/disables the debug overlay to be drawn and used
 *
 * @param[in] renderer
 *      a Handle to the renderer
 */
void c8ToggleDebugOverlay(C8Renderer *renderer);

/**
 * Checks whether the debug overlay is enabled
 *
 * @param[in] renderer
 *      A handle to the renderer
 *
 * @return
 *      `C8_TRUE`, if the debug overlay is enabled. `C8_FALSE`, otherwise
 */
C8Bool c8DebugOverlayEnabled(const C8Renderer *renderer);

/**
 * Adds some formatted debugging text to the internal debug overlay
 *
 * @param[in] renderer
 *      A handle to the renderer
 * @param[in] fmt
 *      The format specifier
 */
void c8AddDebugText(C8Renderer *renderer, const char *fmt, ...);

#endif /* RENDERER_H */

