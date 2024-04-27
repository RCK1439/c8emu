#include "debug.h"

#ifndef NDEBUG
#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_SIZE 256

static void print_opcode(FILE *f, opcode_t op);

void disassemble(const char *rom_name, uint8_t *prog, size_t size)
{
    char log_file_name[FILENAME_SIZE], *last_pos;
    FILE *log_file;
    size_t i, length;
    uint16_t hex_code;
    opcode_t opcode;

    if ((last_pos = strrchr(rom_name, '\\')) != NULL) {
        length = strnlen(++last_pos, FILENAME_SIZE);
        snprintf(log_file_name, FILENAME_SIZE, "%s", last_pos);
    } else {
        length = strnlen(rom_name, FILENAME_SIZE);
        snprintf(log_file_name, FILENAME_SIZE, "%s", rom_name);
    }
    snprintf(log_file_name + length - 4, FILENAME_SIZE, "%s", ".dis");

    if ((log_file = fopen(log_file_name, "w")) == NULL) {
        fprintf(stderr, "failed to create %s\n", log_file_name);
        exit(1);
    }

    fprintf(log_file, ".code\n");
    for (i = 0; i < size; i += 2) {
        hex_code = ((uint16_t)prog[i] << 8) | ((uint16_t)prog[i+1]);
        opcode = decode_opcode(hex_code);
        print_opcode(log_file, opcode);
    }

    fclose(log_file);
}

void debug_opcode(opcode_t *op)
{
    const char *in_str[] = {
        "IN_RAW",
        "IN_CLS",
        "IN_RET",
        "IN_SYS",
        "IN_JP",
        "IN_CALL",
        "IN_SE",
        "IN_SNE",
        "IN_LD",
        "IN_ADD",
        "IN_OR",
        "IN_AND",
        "IN_XOR",
        "IN_SUB",
        "IN_SHR",
        "IN_SUBN",
        "IN_SHL",
        "IN_RND",
        "IN_DRW",
        "IN_SKP",
        "IN_SKNP"
    };

    const char *am_str[] = {
        "AM_NONE",
        "AM_OPCODE",
        "AM_ADDR",
        "AM_VX_BYTE",
        "AM_VX_VY",
        "AM_I_ADDR",
        "AM_V0_ADDR",
        "AM_VX_VY_N",
        "AM_VX",
        "AM_VX_DT",
        "AM_VX_KEY",
        "AM_DT_VX",
        "AM_ST_VX",
        "AM_I_VX",
        "AM_FONT_VX",
        "AM_BCD_VX",
        "AM_ADDR_I_VX",
        "AM_VX_ADDR_I"
    };

    printf("opcode:\n");
    printf(" - instruction: %s\n", in_str[op->instr]);
    printf(" - address mode: %s\n", am_str[op->addr_mode]);
    printf(" - vx: %d\n", op->x_reg);
    printf(" - vy: %d\n", op->y_reg);
    printf(" - kk: %d\n", op->byte);
    printf(" - n: %d\n", op->nibble);
    printf(" - nnn: %d\n", op->address);
    printf(" - raw: %d\n", op->raw);
}

static void print_opcode(FILE *f, opcode_t op)
{
    const char *in_str[] = {
        "raw", "cls", "ret",
        "sys", "jp", "call",
        "se", "sne", "ld",
        "add", "or", "and",
        "xor", "sub", "shr",
        "subn", "shl", "rnd",
        "drw", "skp", "sknp"
    };

    switch (op.addr_mode) {
        case AM_NONE: fprintf(f, "  %s\n", in_str[op.instr]); break;
        case AM_OPCODE: fprintf(f, "  %s 0x%04X\n", in_str[op.instr], op.raw); break;
        case AM_ADDR: fprintf(f, "  %s 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_VX_BYTE: fprintf(f, "  %s v%01X, 0x%02X\n", in_str[op.instr], op.x_reg, op.byte); break;
        case AM_VX_VY: fprintf(f, "  %s v%01X, v%01x\n", in_str[op.instr], op.x_reg, op.y_reg); break;
        case AM_I_ADDR: fprintf(f, "  %s i, 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_V0_ADDR: fprintf(f, "  %s v0, 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_VX_VY_N: fprintf(f, "  %s v%01X, v%01X, 0x%02X\n", in_str[op.instr], op.x_reg, op.y_reg, op.nibble); break;
        case AM_VX: fprintf(f, "  %s v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_VX_DT: fprintf(f, "  %s v%01X, dt\n", in_str[op.instr], op.x_reg); break;
        case AM_VX_KEY: fprintf(f, "  %s v%01X, key\n", in_str[op.instr], op.x_reg); break;
        case AM_DT_VX: fprintf(f, "  %s dt, v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_ST_VX: fprintf(f, "  %s st, v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_I_VX: fprintf(f, "  %s i, v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_FONT_VX: fprintf(f, "  %s font, v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_BCD_VX: fprintf(f, "  %s bcd, v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_ADDR_I_VX: fprintf(f, "  %s [i], v%01X\n", in_str[op.instr], op.x_reg); break;
        case AM_VX_ADDR_I: fprintf(f, "  %s v%01X, [i]\n", in_str[op.instr], op.x_reg); break;
    }
}

#endif