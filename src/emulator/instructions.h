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
    AM_NONE      = 0,
    AM_OPCODE    = 1 << 0,
    AM_ADDR      = 1 << 1,
    AM_VX_BYTE   = 1 << 2,
    AM_VX_VY     = 1 << 3,
    AM_I_ADDR    = 1 << 4,
    AM_V0_ADDR   = 1 << 5,
    AM_VX_VY_N   = 1 << 6,
    AM_VX        = 1 << 7,
    AM_VX_DT     = 1 << 8,
    AM_VX_KEY    = 1 << 9,
    AM_DT_VX     = 1 << 10,
    AM_ST_VX     = 1 << 11,
    AM_I_VX      = 1 << 12,
    AM_FONT_VX   = 1 << 13,
    AM_BCD_VX    = 1 << 14,
    AM_ADDR_I_VX = 1 << 15,
    AM_VX_ADDR_I = 1 << 16
} C8AddrMode;

/**
 * Arguments used for the AM_VX_BYTE address mode
 */
typedef struct C8VxByte
{
    u8 x, byte;
} C8VxByte;

/**
 * Arguments used for the AM_VX_VY address mode
 */
typedef struct C8VxVy
{
    u8 x, y;
} C8VxVy;

/**
 * Arguments used for the AM_VX_VY_N address mode
 */
typedef struct C8VxVyN
{
    u8 x, y, n;
} C8VxVyN;

/**
 * Arguments used for the AM_VX_ADDR address mode
 */
typedef struct C8VxAddr
{
    u16 addr;
    u8 x;
} C8VxAddr;

/**
 * Union containing the address mode arguments
 */
typedef union C8Args
{
    u16      address; // 16-bit memory address
    C8VxByte vxByte;
    C8VxVy   vxVy;
    C8VxVyN  vxVyN;
    C8VxAddr vxAddr;
    u16      raw;    // Raw opcode
    u8       x;      // X-register index
} C8Args;

/**
 * Struct representing the decoded opcode read from the ROM file
 */
typedef struct C8OpCode
{
    C8Instr    instr;       // The instruction
    C8AddrMode addressMode; // The mode which the instruction must be addressed with
    C8Args     args;        // The arguments based on the address mode
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
