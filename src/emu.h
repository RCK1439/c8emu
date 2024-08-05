/**
 * @file   emu.h
 * @brief  Provides an interface for the Chip-8 emulator.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef EMU_H
#define EMU_H

#include <stdint.h>

/* --- type definitions ---------------------------------------------------- */

typedef enum emulator_status_s {
    EMU_OK,
    EMU_USAGE_ERR,
    EMU_RAM_ERR,
} emulator_status_t;

/* --- emulator interface -------------------------------------------------- */

/**
 * Initializes and runs the Chip-8 emulator
 * 
 * @param[in] argc
 *      The amount of arguments passed via the command-line.
 * @param[in] argv
 *      The arguments passed via the command-line.
 * 
 * @return
 *      The exit status of the emulator.
 */
emulator_status_t emu_run(int32_t argc, char **argv);

#endif /* EMU_H */
