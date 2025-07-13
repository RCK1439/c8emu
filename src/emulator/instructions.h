#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "core/types.h"

// --- type definitions -------------------------------------------------------

typedef enum Chip8Instr
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
} Chip8Instr;

typedef enum Chip8AddrMode
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
} Chip8AddrMode;

typedef struct Chip8OpCode
{
    Chip8Instr instr;
    Chip8AddrMode addressMode;
    u16 address;
    u16 raw;
    u8 x;
    u8 y;
    u8 byte;
    u8 nibble; 
} Chip8OpCode;

Chip8OpCode c8DecodeOpCode(u16 op);

#endif /* INSTRUCTIONS_H */

