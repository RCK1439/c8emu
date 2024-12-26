#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

typedef enum Instruction
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
} Instruction;

typedef enum AddressMode
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
} AddressMode;

typedef struct OpCode
{
    Instruction instr;
    AddressMode addressMode;
    
    uint16_t address;
    uint16_t raw;

    uint8_t x;
    uint8_t y;
    uint8_t byte;
    uint8_t nibble;
} OpCode;

OpCode DecodeOpCode(uint16_t op);

#endif /* INSTRUCTIONS_H */

