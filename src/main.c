/**
 * @file   main.c
 * @brief  A basic emulator (or interpreter) for Chip-8
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#include "emu.h"

/* --- main routine -------------------------------------------------------- */

/**
 * Entry point to the emulator. This will simply load the provided ROM file
 * from the command-line, initialize various systems and then run the emulator.
 * 
 * @param[in] argc
 *      The amount of arguments passed via the command-line.
 * @param[in] argv
 *      The arguments passed via the command-line.
 * 
 * @return
 *      The exit status of the emulator.
 */
int main(int argc, char **argv) {
    return emu_run(argc, argv);
}
