#ifndef DEBUG_OVERLAY_H
#define DEBUG_OVERLAY_H

#include <raylib.h>

#include <stdarg.h>
#include <stddef.h>

// --- constants --------------------------------------------------------------

#define MAX_DEBUG_TEXT_SIZE (128)

// --- type definitions -------------------------------------------------------

/**
 * This struct represents the debug text
 */
typedef struct C8DebugText
{
    char    text[MAX_DEBUG_TEXT_SIZE+1]; // Text buffer
    Vector2 position;                    // Position on screen to draw text to
} C8DebugText;

/**
 * Handle definition for debug overlay
 */
typedef struct C8DebugOverlay C8DebugOverlay;

// --- debug overlay interface ------------------------------------------------

/**
 * Creates a new debug overlay
 *
 * @return
 *      A pointer to the newly created debug overlay
 */
C8DebugOverlay *c8CreateDebugOverlay(void);

/**
 * Frees all resources used by the debug overlay
 *
 * @param[in] debugOverlay
 *      A handle to the debug overlay
 */
void c8FreeDebugOverlay(C8DebugOverlay *debugOverlay);

/** 
 * Resets the overlay such that when adding more text reallocations won't
 * occur.
 *
 * NOTE: This MUST be called every frame after drawing the text, otherwise a
 *       leak will occur
 *
 * @param[in] debugOverlay
 *      A handle to the debug overlay
 */
void c8ClearDebugOverlay(C8DebugOverlay *debugOverlay);

/**
 * Adds a new debug text element to the debug overlay. This requires that a
 * function with variadic arguments is the caller
 *
 * @param[in] debugOverlay
 *      A handle to the debug overlay
 * @param[in] fmt
 *      The format specifier
 * @param[in] args
 *      The variadic argument list
 */
void c8AppendDebugText(C8DebugOverlay *debugOverlay, const char *fmt, va_list args);

/**
 * Gets the number of debug text elements in the debug overlay
 *
 * @param[in] debugOverlay
 *      A handle to the debug overlay
 *
 * @return
 *      The size of the debug overlay
 */
size_t c8GetDebugOverlaySize(const C8DebugOverlay *debugOverlay);

/**
 * Gets the debug text element at index `i`
 *
 * @param[in] debugOverlay
 *      A handle to the debug overlay
 * @param[in] i
 *      The index of the element to retrieve
 *
 * @return
 *      The debug text element contained at index `i`
 */
C8DebugText c8GetDebugTextAt(const C8DebugOverlay *debugOverlay, size_t i);

#endif /* DEBUG_OVERLAY_H */

