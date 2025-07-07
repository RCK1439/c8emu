#include "debug.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_KEYS 16
#define NUM_REGISTERS 16

#define FILENAME_SIZE 256
#define FONTSIZE 20

#if defined(C8_DEBUG)

static void c8LogOpCode(FILE *f, Chip8OpCode op);

void c8Disassemble(const char *romName, const u8 *prog, size_t size)
{
    char logFileName[FILENAME_SIZE] = { 0 }, *lastPos = NULL;
    size_t length = 0;

    if ((lastPos = strrchr(romName, '\\')) != NULL)
    {
        length = strlen(++lastPos);
        snprintf(logFileName, FILENAME_SIZE, "%s", lastPos);
    }
    else
    {
        length = strlen(romName);
        snprintf(logFileName, FILENAME_SIZE, "%s", romName);
    }
    snprintf(logFileName + length - 4, FILENAME_SIZE, "%s", ".dis");

    FILE *logFile = NULL;
    if ((logFile = fopen(logFileName, "w")) == NULL)
    {
        fprintf(stderr, "failed to create %s\n", logFileName);
        exit(1);
    }

    fprintf(logFile, ".code\n");
    for (size_t i = 0; i < size; i += 2)
    {
        const u16 hexCode = ((u16)prog[i] << 8) | ((u16)prog[i+1]);
        const Chip8OpCode opcode = c8DecodeOpCode(hexCode);
        c8LogOpCode(logFile, opcode);
    }

    fclose(logFile);
}

void c8DebugOpCode(const Chip8OpCode *op)
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
    printf(" - address mode: %s\n", am_str[op->addressMode]);
    printf(" - vx: %d\n", op->x);
    printf(" - vy: %d\n", op->y);
    printf(" - kk: %d\n", op->byte);
    printf(" - n: %d\n", op->nibble);
    printf(" - nnn: %d\n", op->address);
    printf(" - raw: %d\n", op->raw);
}

void c8DrawDebugInfo(const u8 *v, u8 dt, u8 st, u16 idx, u16 pc, const u8 *kp)
{
    i32 offset = MeasureText("Registers: ", FONTSIZE) + 10;
    
    DrawText("Registers: ", 5, 5, FONTSIZE, WHITE);
    for (u8 i = 0; i < NUM_REGISTERS; i++)
    {
        DrawText(TextFormat("V%X=%d", i, v[i]), 5, 25 + i * FONTSIZE, FONTSIZE, WHITE);
    }

    DrawText("Special Registers: ", offset, 5, FONTSIZE, WHITE);
    DrawText(TextFormat("PC=0x%3X", pc), offset, 25, FONTSIZE, WHITE);
    DrawText(TextFormat("IDX=0x%3X", idx), offset, 45, FONTSIZE, WHITE);
    offset += MeasureText("Special Registers: ", FONTSIZE) + 10;

    DrawText("Timers: ", offset, 5, FONTSIZE, WHITE);
    DrawText(TextFormat("ST=%d", st), offset, 25, FONTSIZE, WHITE);
    DrawText(TextFormat("DT=%d", dt), offset, 45, FONTSIZE, WHITE);
    offset += MeasureText("Timers: ", FONTSIZE) + 10;

    DrawText("Keypad: ", offset, 5, FONTSIZE, WHITE);
    for (u8 i = 0; i < NUM_KEYS; i++)
    {
        DrawText(TextFormat("%X=%d", i, kp[i]), offset, 25 + i * FONTSIZE, FONTSIZE, WHITE);
    }

    const i32 fps = GetFPS();
    SetWindowTitle(TextFormat("c8emu - %d FPS", fps));
}

static void c8LogOpCode(FILE *f, Chip8OpCode op)
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

    switch (op.addressMode)
    {
        case AM_NONE: fprintf(f, "  %s\n", in_str[op.instr]); break;
        case AM_OPCODE: fprintf(f, "  %s 0x%04X\n", in_str[op.instr], op.raw); break;
        case AM_ADDR: fprintf(f, "  %s 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_VX_BYTE: fprintf(f, "  %s v%01X, 0x%02X\n", in_str[op.instr], op.x, op.byte); break;
        case AM_VX_VY: fprintf(f, "  %s v%01X, v%01x\n", in_str[op.instr], op.x, op.y); break;
        case AM_I_ADDR: fprintf(f, "  %s i, 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_V0_ADDR: fprintf(f, "  %s v0, 0x%03X\n", in_str[op.instr], op.address); break;
        case AM_VX_VY_N: fprintf(f, "  %s v%01X, v%01X, 0x%02X\n", in_str[op.instr], op.x, op.y, op.nibble); break;
        case AM_VX: fprintf(f, "  %s v%01X\n", in_str[op.instr], op.x); break;
        case AM_VX_DT: fprintf(f, "  %s v%01X, dt\n", in_str[op.instr], op.x); break;
        case AM_VX_KEY: fprintf(f, "  %s v%01X, key\n", in_str[op.instr], op.x); break;
        case AM_DT_VX: fprintf(f, "  %s dt, v%01X\n", in_str[op.instr], op.x); break;
        case AM_ST_VX: fprintf(f, "  %s st, v%01X\n", in_str[op.instr], op.x); break;
        case AM_I_VX: fprintf(f, "  %s i, v%01X\n", in_str[op.instr], op.x); break;
        case AM_FONT_VX: fprintf(f, "  %s font, v%01X\n", in_str[op.instr], op.x); break;
        case AM_BCD_VX: fprintf(f, "  %s bcd, v%01X\n", in_str[op.instr], op.x); break;
        case AM_ADDR_I_VX: fprintf(f, "  %s [i], v%01X\n", in_str[op.instr], op.x); break;
        case AM_VX_ADDR_I: fprintf(f, "  %s v%01X, [i]\n", in_str[op.instr], op.x); break;
    }
}
#endif

