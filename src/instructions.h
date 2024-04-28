/**
 * @file   instructions.h
 * @brief  Provides types and functions related to opcodes and instructions.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

/* --- type definitions ---------------------------------------------------- */

/**
 * This enum describes the different assembly instructions that the Chip-8
 * can execute.
 */
typedef enum instructions_s {
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
} instructions_t;

/**
 * This enum describes the different addressing modes for some of the
 * instructions.
 */
typedef enum address_mode_s {
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
} address_mode_t;

/**
 * Defines an opcode with its necessary parameters in order to be executed by
 * the CPU.
 */
typedef struct opcode_s {
    instructions_t instr;
    address_mode_t addr_mode;
    
    uint8_t x_reg;
    uint8_t y_reg;
    uint8_t byte;
    uint8_t nibble;

    uint16_t address;
    uint16_t raw;
} opcode_t;

/* --- instructions interface ---------------------------------------------- */

/**
 * Decodes a raw opcode found in the ROM file into an opcode struct.
 * 
 * @param[in] op
 *      The raw opcode bytes found in the ROM file.
 * 
 * @return
 *      The decoded opcode.
 */
opcode_t decode_opcode(uint16_t op);

#endif /* INSTRUCTIONS_H */
