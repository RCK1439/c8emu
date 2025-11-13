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
    AM_NONE      = 0x00001,
    AM_OPCODE    = 0x00002,
    AM_ADDR      = 0x00004,
    AM_VX_BYTE   = 0x00008,
    AM_VX_VY     = 0x00010,
    AM_I_ADDR    = 0x00020,
    AM_V0_ADDR   = 0x00040,
    AM_VX_VY_N   = 0x00080,
    AM_VX        = 0x00100,
    AM_VX_DT     = 0x00200,
    AM_VX_KEY    = 0x00400,
    AM_DT_VX     = 0x00800,
    AM_ST_VX     = 0x01000,
    AM_I_VX      = 0x02000,
    AM_FONT_VX   = 0x04000,
    AM_BCD_VX    = 0x08000,
    AM_ADDR_I_VX = 0x10000,
    AM_VX_ADDR_I = 0x20000
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
 * @param[in] raw
 *      The raw opcode to decode
 *
 * @return
 *      The decoded opcode
 */
C8OpCode c8DecodeOpCode(u16 raw);

#endif /* INSTRUCTIONS_H */
