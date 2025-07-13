#ifndef RAM_H
#define RAM_H

#include "rom.h"
#include "specifications.h"

// --- type definitions -------------------------------------------------------

/**
 * Struct representing the system memory of the Chip-8
 */
typedef struct C8RAM
{
    u8 memory[C8_MEMORY_SIZE]; // The memory buffer
} C8RAM;

// --- ram interface ----------------------------------------------------------

/**
 * Zero-initializes the RAM and loads system font
 *
 * @return
 *      The initialized memory
 */
C8RAM c8InitRAM(void);

/**
 * Loads the given ROM into the Chip-8 memory
 *
 * @param[in] ram
 *      A pointer to the Chip-8 RAM
 * @param[in] rom
 *      The ROM to load into the memory
 */
void c8UploadROMToRAM(C8RAM *ram, C8ROM rom);

/**
 * Writes `val` into the memory located at `addr` in the Chip-8 RAM
 *
 * @param[in] ram
 *      A pointer to the Chip-8 RAM
 * @param[in] addr
 *      The memory address to write to
 * @param[in] val
 *      The value to write to the memory address
 */
void c8RAMWrite(C8RAM *ram, u16 addr, u8 val);

/**
 * Reads the value from the memory located at `addr` from the Chip-8 RAM
 *
 * @param[in] ram
 *      A pointer to the Chip-8 RAM
 * @param[in] addr
 *      The memory address to read from
 *
 * @return
 *      The value at the specified memory address
 */
u8 c8RAMRead(const C8RAM *ram, u16 addr);

#endif /* RAM_H */
