/**
 * @file   ram.h
 * @brief  Provides an interface for the CPU to interact with the Chip-8
 *         memory.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef RAM_H
#define RAM_H

#include <stdint.h>

/* --- type definitions ---------------------------------------------------- */

/**
 * This enum describes the ram status when initializing.
 */
typedef enum ram_status_s {
    RAM_OK,
    RAM_FILE_ERR,
    RAM_ALLOC_ERR
} ram_status_t;

/* --- ram interface ------------------------------------------------------- */

/**
 * Initializes the memory and loads in the rom file.
 * 
 * @param[in] rom_file
 *      The filepath to the ROM to load into memory.
 * 
 * @return
 *      The status code after the ram has been initialized.
 */
ram_status_t ram_init(const char *rom_file);

/**
 * Writes `val` to memory at `addr`.
 * 
 * @param[in] addr
 *      The address to write the value to.
 * @param[in] val
 *      The value to write to memory.
 */
void ram_write(uint16_t addr, uint8_t val);

/**
 * Reads the contents of the memory at `addr`.
 * 
 * @param[in] addr
 *      The memory address to read from.
 *
 * @return
 *      The value at `addr`.
 */
uint8_t ram_read(uint16_t addr);

#endif /* RAM_H */