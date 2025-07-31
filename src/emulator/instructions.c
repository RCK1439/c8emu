#include "instructions.h"

// --- useful macros ----------------------------------------------------------

#define INSTR(op) (((op) & 0xF000) >> 12)
#define X(op)     (((op) & 0x0F00) >> 8)
#define Y(op)     (((op) & 0x00F0) >> 4)
#define N(op)     ((op) & 0x000F)
#define KK(op)    ((op) & 0x00FF)
#define NNN(op)   ((op) & 0x0FFF)

// --- type definitions -------------------------------------------------------

typedef C8OpCode (*C8DecodeProc)(u16);

// --- decoder functions ------------------------------------------------------

static C8OpCode c8Decode0(u16 raw);
static C8OpCode c8Decode1(u16 raw);
static C8OpCode c8Decode2(u16 raw);
static C8OpCode c8Decode3(u16 raw);
static C8OpCode c8Decode4(u16 raw);
static C8OpCode c8Decode5(u16 raw);
static C8OpCode c8Decode6(u16 raw);
static C8OpCode c8Decode7(u16 raw);
static C8OpCode c8Decode8(u16 raw);
static C8OpCode c8Decode9(u16 raw);
static C8OpCode c8DecodeA(u16 raw);
static C8OpCode c8DecodeB(u16 raw);
static C8OpCode c8DecodeC(u16 raw);
static C8OpCode c8DecodeD(u16 raw);
static C8OpCode c8DecodeE(u16 raw);
static C8OpCode c8DecodeF(u16 raw);

static const C8DecodeProc s_Decoders[] = {
    c8Decode0, c8Decode1, c8Decode2, c8Decode3,
    c8Decode4, c8Decode5, c8Decode6, c8Decode7,
    c8Decode8, c8Decode9, c8DecodeA, c8DecodeB,
    c8DecodeC, c8DecodeD, c8DecodeE, c8DecodeF
};

// --- instructions implementation --------------------------------------------

C8OpCode c8DecodeOpCode(u16 raw)
{
    const size_t idx = (size_t)((raw >> 12) & 0x000F);
    return s_Decoders[idx](raw);
}

// --- decoder implementation -------------------------------------------------

static C8OpCode c8Decode0(u16 raw)
{
    C8OpCode code = { 0 };

    switch (raw & 0x00FF)
    {
        case 0xE0:
        {
            code.instr = IN_CLS;
            code.addressMode = AM_NONE;
        } break;
        case 0xEE:
        {
            code.instr = IN_RET;
            code.addressMode = AM_NONE;
        } break;
        default:
        {
            code.instr = IN_RAW;
            code.addressMode = AM_OPCODE;
            code.raw = raw;
        } break;
    }

    return code;
}

static C8OpCode c8Decode1(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_JP,
        .addressMode = AM_ADDR,
        .address = NNN(raw)
    };

    return code;
}

static C8OpCode c8Decode2(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_CALL,
        .addressMode = AM_ADDR,
        .address = NNN(raw),
    };

    return code;
}

static C8OpCode c8Decode3(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_SE,
        .addressMode = AM_VX_BYTE,
        .x = X(raw),
        .byte = KK(raw),
    };

    return code;
}

static C8OpCode c8Decode4(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_SNE,
        .addressMode = AM_VX_BYTE,
        .x = X(raw),
        .byte = KK(raw),
    };

    return code;
}

static C8OpCode c8Decode5(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_SE,
        .addressMode = AM_VX_VY,
        .x = X(raw),
        .y = Y(raw),
    };

    return code;
}

static C8OpCode c8Decode6(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_LD,
        .addressMode = AM_VX_BYTE,
        .x = X(raw),
        .byte = KK(raw),
    };

    return code;
}

static C8OpCode c8Decode7(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_ADD,
        .addressMode = AM_VX_BYTE,
        .x = X(raw),
        .byte = KK(raw),
    };

    return code;
}

static C8OpCode c8Decode8(u16 raw)
{
    C8OpCode code = {
        .addressMode = AM_VX_VY,
        .x = X(raw),
        .y = Y(raw),
    };

    switch (raw & 0x000F)
    {
        case 0x0: code.instr = IN_LD; break;
        case 0x1: code.instr = IN_OR; break;
        case 0x2: code.instr = IN_AND; break;
        case 0x3: code.instr = IN_XOR; break;
        case 0x4: code.instr = IN_ADD; break;
        case 0x5: code.instr = IN_SUB; break;
        case 0x6: code.instr = IN_SHR; break;
        case 0x7: code.instr = IN_SUBN; break;
        case 0xE: code.instr = IN_SHL; break;
        default:
        {
            code.instr = IN_RAW;
            code.addressMode = AM_OPCODE;
            code.raw = raw;
        } break;
    }

    return code;
}

static C8OpCode c8Decode9(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_SNE,
        .addressMode = AM_VX_VY,
        .x = X(raw),
        .y = Y(raw),
    };

    return code;
}

static C8OpCode c8DecodeA(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_LD,
        .addressMode = AM_I_ADDR,
        .address = NNN(raw),
    };
    
    return code;
}

static C8OpCode c8DecodeB(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_JP, 
        .addressMode = AM_V0_ADDR,
        .address = NNN(raw),
    };

    return code;
}

static C8OpCode c8DecodeC(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_RND,
        .addressMode = AM_VX_BYTE,
        .x = X(raw),
        .byte = KK(raw),
    };
    
    return code;
}

static C8OpCode c8DecodeD(u16 raw)
{
    const C8OpCode code = {
        .instr = IN_DRW,
        .addressMode = AM_VX_VY_N,
        .x = X(raw),
        .y = Y(raw),
        .nibble = N(raw),
    };
    
    return code;
}

static C8OpCode c8DecodeE(u16 raw)
{
    C8OpCode code = { 0 };

    switch (raw & 0x00FF)
    {
        case 0x9E:
        {
            code.instr = IN_SKP;
            code.addressMode = AM_VX;
            code.x = X(raw);
        } break;
        case 0xA1:
        {
            code.instr = IN_SKNP;
            code.addressMode = AM_VX;
            code.x = X(raw);
        } break;
        default:
        {
            code.instr = IN_RAW;
            code.addressMode = AM_OPCODE;
            code.raw = raw;
        } break;
    }

    return code;
}

static C8OpCode c8DecodeF(u16 raw)
{
    C8OpCode code = { 0 };

    switch (raw & 0x00FF)
    {
        case 0x07:
        {
            code.instr = IN_LD;
            code.addressMode = AM_VX_DT;
            code.x = X(raw);
        } break;
        case 0x0A:
        {
            code.instr = IN_LD;
            code.addressMode = AM_VX_KEY;
            code.x = X(raw);
        } break;
        case 0x15:
        {
            code.instr = IN_LD;
            code.addressMode = AM_DT_VX;
            code.x = X(raw);
        } break;
        case 0x18:
        {
            code.instr = IN_LD;
            code.addressMode = AM_ST_VX;
            code.x = X(raw);
        } break;
        case 0x1E:
        {
            code.instr = IN_ADD;
            code.addressMode = AM_I_VX;
            code.x = X(raw);
        } break;
        case 0x29:
        {
            code.instr = IN_LD;
            code.addressMode = AM_FONT_VX;
            code.x = X(raw);
        } break;
        case 0x33:
        {
            code.instr = IN_LD;
            code.addressMode = AM_BCD_VX;
            code.x = X(raw);
        } break;
        case 0x55:
        {
            code.instr = IN_LD;
            code.addressMode = AM_ADDR_I_VX;
            code.x = X(raw);
        } break;
        case 0x65:
        {
            code.instr = IN_LD;
            code.addressMode = AM_VX_ADDR_I;
            code.x = X(raw);
        } break;
        default:
        {
            code.instr = IN_RAW;
            code.addressMode = AM_OPCODE;
            code.raw = raw;
        } break;
    }

    return code;
}

