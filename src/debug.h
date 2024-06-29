/**
 * @file   debug.h
 * @brief  Provides a debugging interface for the emulator.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
#include "instructions.h"

#include <stddef.h>
#include <stdint.h>

/* --- macros -------------------------------------------------------------- */

#define DISASSEMBLE(rom_name, prog, size) disassemble(rom_name, prog, size)
#define DEBUG_OPCODE(op) debug_opcode(op)
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp) draw_debug_info(v, dt, st, idx, pc, kp);

/* --- debug interface ----------------------------------------------------- */

/**
 * Disassembles the ROM bytes and saves it to a file.
 * 
 * @param[in] rom_name
 *      The name of the ROM file.
 * @param[in] prog
 *      The loaded ROM bytes from the ROM file.
 * @param[in] size
 *      The size of the loaded ROM.
 */
void disassemble(const char *rom_name, const uint8_t *const prog, size_t size);

/**
 * Prints the entire opcode to `stdout`.
 * 
 * @param[in] op
 *      A pointer to the opcode to debug.
 */
void debug_opcode(const opcode_t *const op);

/**
 * Draws debugging information about the CPU to the screen.
 * 
 * @param[in] v
 *      The registers of the CPU.
 * @param[in] dt
 *      The delay timer.
 * @param[in] st
 *      The sound timer.
 * @param[in] idx
 *      The index register.
 * @param[in] pc
 *      The program counter.
 * @param[in] kp
 *      The keypad.
 */
void draw_debug_info(const uint8_t *const v, uint8_t dt, uint8_t st, uint16_t idx, uint16_t pc, const uint8_t *const kp);

#else
#define DISASSEMBLE(rom_name, prog, size)
#define DEBUG_OPCODE(op)
#define DRAW_DEBUG_INFO(v, dt, st, idx, pc, kp)
#endif

#endif /* DEBUG_H */
