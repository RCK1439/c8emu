#include "instructions.h"

#define INSTR(op) (((op) & 0xF000) >> 12)
#define X(op) (((op) & 0x0F00) >> 8)
#define Y(op) (((op) & 0x00F0) >> 4)
#define N(op) ((op) & 0x000F)
#define KK(op) ((op) & 0x00FF)
#define NNN(op) ((op) & 0x0FFF)

OpCode DecodeOpCode(uint16_t op)
{
    OpCode code = { 0 };

    switch (op & 0xF000)
    {
        case 0x0000:
        {
            switch (op & 0x00FF)
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
                    code.raw = op;
                } break;
            }
        } return code;
        case 0x1000:
        {
            code.instr = IN_JP;
            code.addressMode = AM_ADDR;
            code.address = NNN(op);
        } return code;
        case 0x2000:
        {
            code.instr = IN_CALL;
            code.addressMode = AM_ADDR;
            code.address = NNN(op);
        } return code;
        case 0x3000:
        {
            code.instr = IN_SE;
            code.addressMode = AM_VX_BYTE;
            code.x = X(op);
            code.byte = KK(op);
        } return code;
        case 0x4000:
        {
            code.instr = IN_SNE;
            code.addressMode = AM_VX_BYTE;
            code.x = X(op);
            code.byte = KK(op);
        } return code;
        case 0x5000:
        {
            code.instr = IN_SE;
            code.addressMode = AM_VX_VY;
            code.x = X(op);
            code.y = Y(op);
        } return code;
        case 0x6000:
        {
            code.instr = IN_LD;
            code.addressMode = AM_VX_BYTE;
            code.x = X(op);
            code.byte = KK(op);
        } return code;
        case 0x7000:
        {
            code.instr = IN_ADD;
            code.addressMode = AM_VX_BYTE;
            code.x = X(op);
            code.byte = KK(op);
        } return code;
        case 0x8000:
        {
            code.addressMode = AM_VX_VY;
            code.x = X(op);
            code.y = Y(op);
            
            switch (op & 0x000F)
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
                    code.raw = op;
                } break;
            }
        } return code;
        case 0x9000:
        {
            code.instr = IN_SNE;
            code.addressMode = AM_VX_VY;
            code.x = X(op);
            code.y = Y(op);
        } return code;
        case 0xA000:
        {
            code.instr = IN_LD;
            code.addressMode = AM_I_ADDR;
            code.address = NNN(op);
        } return code;
        case 0xB000:
        {
            code.instr = IN_JP;
            code.addressMode = AM_V0_ADDR;
            code.address = NNN(op);
        } return code;
        case 0xC000:
        {
            code.instr = IN_RND;
            code.addressMode = AM_VX_BYTE;
            code.x = X(op);
            code.byte = KK(op);
        } return code;
        case 0xD000:
        {
            code.instr = IN_DRW;
            code.addressMode = AM_VX_VY_N;
            code.x = X(op);
            code.y = Y(op);
            code.nibble = N(op);
        } return code;
        case 0xE000:
        {
            switch (op & 0x00FF)
            {
                case 0x9E:
                {
                    code.instr = IN_SKP;
                    code.addressMode = AM_VX;
                    code.x = X(op);
                } break;
                case 0xA1:
                {
                    code.instr = IN_SKNP;
                    code.addressMode = AM_VX;
                    code.x = X(op);
                } break;
                default:
                {
                    code.instr = IN_RAW;
                    code.addressMode = AM_OPCODE;
                    code.raw = op;
                } break;
            }
        } return code;
        case 0xF000:
        {
            switch (op & 0x00FF)
            {
                case 0x07:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_VX_DT;
                    code.x = X(op);
                } break;
                case 0x0A:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_VX_KEY;
                    code.x = X(op);
                } break;
                case 0x15:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_DT_VX;
                    code.x = X(op);
                } break;
                case 0x18:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_ST_VX;
                    code.x = X(op);
                } break;
                case 0x1E:
                {
                    code.instr = IN_ADD;
                    code.addressMode = AM_I_VX;
                    code.x = X(op);
                } break;
                case 0x29:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_FONT_VX;
                    code.x = X(op);
                } break;
                case 0x33:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_BCD_VX;
                    code.x = X(op);
                } break;
                case 0x55:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_ADDR_I_VX;
                    code.x = X(op);
                } break;
                case 0x65:
                {
                    code.instr = IN_LD;
                    code.addressMode = AM_VX_ADDR_I;
                    code.x = X(op);
                } break;
                default:
                {
                    code.instr = IN_RAW;
                    code.addressMode = AM_OPCODE;
                    code.raw = op;
                } break;
            }
        } return code;
        default:
        {
            code.instr = IN_RAW;
            code.addressMode = AM_OPCODE;
            code.raw = op;
        } return code;
    }
}

