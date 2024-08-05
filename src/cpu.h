/**
 * @file   cpu.h
 * @brief  Defines the API with which the emulator can interact with the CPU.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef CPU_H
#define CPU_H

#include <stdint.h>

/**
 * Initializes the cpu to its "boot" state.
 */
void cpu_init(void);

/**
 * Performs a single CPU cycle.
 */
void cpu_step(void);

/**
 * Sets the keypad at `key` to `val`.
 * 
 * @param[in] key
 *      The key to set.
 * @param[in] val
 *      The value to set `key` to.
 */
void cpu_set_key(uint8_t key, uint8_t val);

/**
 * Draws the video buffer to the window.
 */
void cpu_draw_buffer(void);

#endif /* CPU_H */

