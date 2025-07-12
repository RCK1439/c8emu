#ifndef CPU_H
#define CPU_H

#include "keyboard.h"
#include "ram.h"
#include "specifications.h"
#include "stack.h"

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * Enum containing indices for the CPU registers
 */
typedef enum Chip8Register
{
    V0 = 0x00,
    V1 = 0x01,
    V2 = 0x02,
    V3 = 0x03,
    V4 = 0x04,
    V5 = 0x05,
    V6 = 0x06,
    V7 = 0x07,
    V8 = 0x08,
    V9 = 0x09,
    VA = 0x0A,
    VB = 0x0B,
    VC = 0x0C,
    VD = 0x0D,
    VE = 0x0E,
    VF = 0x0F
} Chip8Register;

/**
 * This struct represents the Chip-8 processor
 */
typedef struct Chip8CPU
{
    u8             video[C8_SCREEN_BUFFER_WIDTH * C8_SCREEN_BUFFER_HEIGHT]; // The video buffer
    Chip8CallStack stack;                                                   // The call stack
    u8             keypad[C8_NUM_KEYS];                                     // The system keypad flags
    u8             registers[C8_NUM_REGISTERS];                             // The processor registers
    u16            idx;                                                     // The system index register
    u16            pc;                                                      // The system program counter
    u8             dt;                                                      // The delay timer
    u8             st;                                                      // The sound timer
} Chip8CPU;

// --- cpu interface ----------------------------------------------------------

/**
 * Initializes the CPU to it's initial state
 *
 * @return
 *      The initialized CPU
 */
Chip8CPU c8InitCPU(void);

/**
 * Cycles the Chip-8 processor
 *
 * @param[in] cpu
 *      The Chip-8 processor
 * @param[in] ram
 *      The Chip-8 memory
 */
void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram);

/**
 * Sets the key flag located at `key` to `val`
 *
 * @param[in] cpu
 *      The Chip-8 processor
 * @param[in] key
 *      The key index on the cpu
 * @param[in] val
 *      The value to set the key flag to
 */
void c8SetCPUKey(Chip8CPU *cpu, Chip8Key key, u8 val);

#endif /* CPU_H */

