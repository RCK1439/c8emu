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