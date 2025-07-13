#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * All the possible instructions the Chip-8 processor has
 */
typedef enum C8Instr
{
    IN_RAW,
    IN_CLS,
    IN_RET,
    IN_SYS,
    IN_JP,
    IN_CALL,
    IN_SE,
    IN_SNE,
    IN_LD,
    IN_ADD,
    IN_OR,
    IN_AND,
    IN_XOR,
    IN_SUB,
    IN_SHR,
    IN_SUBN,
    IN_SHL,
    IN_RND,
    IN_DRW,
    IN_SKP,
    IN_SKNP
} C8Instr;

/**
 * Enum containing different address modes for different instructions
 */
typedef enum C8AddrMode
{
    AM_NONE,
    AM_OPCODE,
    AM_ADDR,
    AM_VX_BYTE,
    AM_VX_VY,
    AM_I_ADDR,
    AM_V0_ADDR,
    AM_VX_VY_N,
    AM_VX,
    AM_VX_DT,
    AM_VX_KEY,
    AM_DT_VX,
    AM_ST_VX,
    AM_I_VX,
    AM_FONT_VX,
    AM_BCD_VX,
    AM_ADDR_I_VX,
    AM_VX_ADDR_I
} C8AddrMode;

/**
 * Struct representing the decoded opcode read from the ROM file
 */
typedef struct C8OpCode
{
    C8Instr    instr;       // The instruction
    C8AddrMode addressMode; // The mode which the instruction must be addressed with
    u16        address;     // Memory address argument
    u16        raw;         // Raw opcode argument
    u8         x;           // X-register argument
    u8         y;           // Y-register argument
    u8         byte;        // Byte argument
    u8         nibble;      // Nibble argument
} C8OpCode;

// --- instructions interface -------------------------------------------------

/**
 * Decodes a raw opcode from a Chip-8 ROM file
 *
 * @param[in] op
 *      The raw opcode to decode
 *
 * @return
 *      The decoded opcode
 */
C8OpCode c8DecodeOpCode(u16 op);

#endif /* INSTRUCTIONS_H */

