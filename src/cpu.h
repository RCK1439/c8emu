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
void InitCPU(void);

/**
 * Performs a single CPU cycle.
 */
void CPUStep(void);

/**
 * Sets the keypad at `key` to `val`.
 * 
 * @param[in] key
 *      The key to set.
 * @param[in] val
 *      The value to set `key` to.
 */
void CPUSetKey(uint8_t key, uint8_t val);

/**
 * Draws the video buffer to the window.
 */
void CPUDrawBuffer(void);

#endif /* CPU_H */
