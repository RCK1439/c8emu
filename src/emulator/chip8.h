#ifndef CHIP8_H
#define CHIP8_H

#include "renderer/renderer.h"

// --- type definitions -------------------------------------------------------

/**
* Struct representing a handle to the Chip-8 emulator
*/
typedef struct Chip8 Chip8;

// --- chip-8 interface -------------------------------------------------------

/**
 * Initializes the emulator
 *
 * @return
 *      A handle to the initialized emulator
 */
Chip8 *c8InitEmulator(void);

/**
 * Cleans up any resources used by the emulator and closes the emulator
 *
 * @param[in] emulator
 *      The handle to the emulator
 */
void c8CloseEmulator(Chip8 *emulator);

/**
 * Loads the ROM located at `romFile` (on the host machine) into the memory of
 * the emulator
 *
 * @param[in] emulator
 *      The handle to the emulator
 * @param[in] romFile
 *      The filepath to the ROM file to load to the emulator
 */
void c8LoadROMInEmulator(Chip8 *emulator, const char *romFile);

/**
 * Called once per frame to tick the emulator
 *
 * @param[in] emulator
 *      The handle to the emulator
 */
void c8EmulatorOnUpdate(Chip8 *emulator);

/**
 * Called once per frame to render the video buffer of the emulator
 *
 * @param[in] emulator
 *      The handle to the emulator
 * @param[in] renderer
 *      The handle to the renderer
 */
void c8EmulatorOnRender(const Chip8* emulator, Chip8Renderer *renderer);

#endif /* CHIP8_H */
