#ifndef ROM_H
#define ROM_H

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * Struct containing the Chip-8 ROM data and metadata
 */
typedef struct C8ROM
{
    u8         *data;    // The raw ROM data
    size_t      size;    // The number of bytes of the ROM
    const char *romName; // The name of the ROM
} C8ROM;

// --- rom interface ----------------------------------------------------------

/**
 * Loads the ROM located at `romFile` from disk
 *
 * @param[in] romFile
 *      The filepath to the ROM file to load
 *
 * @return
 *      The loaded ROM file
 */
C8ROM c8LoadROM(const char *romFile);

/**
 * Unloads the ROM from memory
 *
 * @param[in] rom
 *      The ROM to unload
 */
void c8UnloadROM(C8ROM rom);

#endif /* ROM_H */
