#include "cpu.h"
#include "emulator/keyboard.h"
#include "instructions.h"
#include "ram.h"
#include "stack.h"

#include "core/debug.h"
#include "core/platform.h"
#include "core/types.h"

#include <raylib.h>
#include <memory.h>

#define C8_ENSURE_ADDR_MODE(addr_mode, expected)                 \
    if ((addr_mode) != (expected))                               \
        c8Panic(ERR_INVALID_ADDRESS_MODE,                        \
            "Unexpected address mode in %s executor routine: %d",\
            __func__,                                            \
            (addr_mode))                                         \

typedef void (*Chip8Exec)(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);

static void c8Raw(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Cls(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Ret(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Sys(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Jp(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Call(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Se(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Sne(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Ld(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Add(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Or(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8And(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Xor(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Sub(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Shr(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Subn(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Shl(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Rnd(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Drw(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Skp(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);
static void c8Sknp(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op);

static const Chip8Exec s_executors[] = {
    [IN_RAW] = c8Raw,
    [IN_CLS] = c8Cls,
    [IN_RET] = c8Ret,
    [IN_SYS] = c8Sys,
    [IN_JP] = c8Jp,
    [IN_CALL] = c8Call,
    [IN_SE] = c8Se,
    [IN_SNE] = c8Sne,
    [IN_LD] = c8Ld,
    [IN_ADD] = c8Add,
    [IN_OR] = c8Or,
    [IN_AND] = c8And,
    [IN_XOR] = c8Xor,
    [IN_SUB] = c8Sub,
    [IN_SHR] = c8Shr,
    [IN_SUBN] = c8Subn,
    [IN_SHL] = c8Shl,
    [IN_RND] = c8Rnd,
    [IN_DRW] = c8Drw,
    [IN_SKP] = c8Skp,
    [IN_SKNP] = c8Sknp
};

Chip8CPU c8InitCPU(void)
{
    Chip8CPU cpu = { 0 };
    cpu.stack = c8InitStack();
    cpu.pc = C8_ADDR_PC;

    return cpu;
}

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram)
{
    for (u8 i = 0; i < C8_OPS_PER_CYCLE; i++)
    {
        const u16 raw = ((u16)c8RAMRead(ram, cpu->pc) << 8) | (u16)c8RAMRead(ram, cpu->pc + 1);
        cpu->pc += 2;

        const Chip8OpCode opcode = c8DecodeOpCode(raw);
        s_executors[opcode.instr](cpu, ram, &opcode);
    }

    if (cpu->dt > 0)
    {
        cpu->dt--;
    }

    if (cpu->st > 0)
    {
        cpu->st--;
    }
}

void c8SetCPUKey(Chip8CPU *cpu, Chip8Key key, u8 val)
{
    cpu->keypad[key] = val;
}

static void c8Raw(UNUSED Chip8CPU *cpu, UNUSED Chip8RAM *ram, UNUSED const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_OPCODE);
    /* Intentionally left empty */
}

static void c8Cls(Chip8CPU *cpu, UNUSED Chip8RAM *ram, UNUSED const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_NONE);
    memset(cpu->video, 0x00, sizeof(cpu->video));
}

static void c8Ret(Chip8CPU *cpu, UNUSED Chip8RAM *ram, UNUSED const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_NONE);
    cpu->pc = c8StackPop(&cpu->stack);
}

static void c8Sys(UNUSED Chip8CPU *cpu, UNUSED Chip8RAM *ram, const UNUSED Chip8OpCode *op)
{
    /* Intentionally left empty */
}

static void c8Jp(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_ADDR:
            cpu->pc = op->address;
            break;
        case AM_V0_ADDR:
            cpu->pc = cpu->v[0x0] + op->address;
            break;
        default:
            C8_ASSERT(C8_FALSE, "Invalid opcode in %s: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Call(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_ADDR);
    c8StackPush(&cpu->stack, cpu->pc);
    cpu->pc = op->address;
}

static void c8Se(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            if (cpu->v[op->x] == op->byte)
            {
                cpu->pc += 2;
            }
            break;
        case AM_VX_VY:
            if (cpu->v[op->x] == cpu->v[op->y])
            {
                cpu->pc += 2;
            }
            break;
        default:
            C8_ASSERT(C8_FALSE, "Invalid opcode in %s: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Sne(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            if (cpu->v[op->x] != op->byte)
            {
                cpu->pc += 2;
            }
            break;
        case AM_VX_VY:
            if (cpu->v[op->x] != cpu->v[op->y])
            {
                cpu->pc += 2;
            }
            break;
        default:
            C8_ASSERT(C8_FALSE, "Invalid opcode in %s: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Ld(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            cpu->v[op->x] = op->byte;
            break;
        case AM_VX_VY:
            cpu->v[op->x] = cpu->v[op->y];
            break;
        case AM_I_ADDR:
            cpu->idx = op->address;
            break;
        case AM_VX_DT:
            cpu->v[op->x] = cpu->dt;
            break;
        case AM_VX_KEY:
        {
            Chip8Bool found = C8_FALSE;
            for (u8 i = 0; i < C8_NUM_KEYS; i++)
            {
                if (cpu->keypad[i])
                {
                    found = C8_TRUE;
                    cpu->v[op->x] = i;
                    break;
                }
            }

            if (!found)
            {
                cpu->pc -= 2;
            }
        } break;
        case AM_DT_VX:
            cpu->dt = cpu->v[op->x];
            break;
        case AM_ST_VX:
            cpu->st = cpu->v[op->x];
            break;
        case AM_FONT_VX:
        {
            const u8 digit = cpu->v[op->x];
            cpu->idx = C8_ADDR_FONT + (5 * digit);
        } break;
        case AM_BCD_VX:
        {
            u8 value = cpu->v[op->x];
            c8RAMWrite(ram, cpu->idx + 2, value % 10);
	        value /= 10;

            c8RAMWrite(ram, cpu->idx + 1, value % 10);
	        value /= 10;

            c8RAMWrite(ram, cpu->idx, value % 10);
        } break;
        case AM_ADDR_I_VX:
            for (u8 i = 0; i <= op->x; i++)
            {
                c8RAMWrite(ram, cpu->idx++, cpu->v[i]);
            }
            break;
        case AM_VX_ADDR_I:
            for (u8 i = 0; i <= op->x; i++)
            {
                cpu->v[i] = c8RAMRead(ram, cpu->idx++);
            }
            break;
        default:
            C8_ASSERT(C8_FALSE, "Invalid opcode in %s: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Add(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
        {
            const u16 sum = (u16)cpu->v[op->x] + (u16)op->byte;
            cpu->v[op->x] = (u8)(sum & 0x00FF);
            cpu->v[0xF] = sum > 0x00FF;
        } break;
        case AM_VX_VY:
        {
            const u16 sum = (u16)cpu->v[op->x] + (u16)cpu->v[op->y];
            cpu->v[op->x] = (u8)(sum & 0x00FF);
            cpu->v[0xF] = sum > 0x00FF;
        } break;
        case AM_I_VX:
            cpu->idx += cpu->v[op->x];
            break;
        default:
            C8_ASSERT(C8_FALSE, "Invalid opcode %s: %d", __func__, op->addressMode);
            break;    
    }
}

static void c8Or(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->v[op->x] |= cpu->v[op->y];
    cpu->v[0xF] = 0;
}

static void c8And(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->v[op->x] &= cpu->v[op->y];
    cpu->v[0xF] = 0;
}

static void c8Xor(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->v[op->x] ^= cpu->v[op->y];
    cpu->v[0xF] = 0;
}

static void c8Sub(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u16 diff = (u16)cpu->v[op->x] - (u16)cpu->v[op->y];
    cpu->v[op->x] = (u8)(diff & 0x00FF);
    cpu->v[0xF] = diff <= 0x00FF;
}

static void c8Shr(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u8 bit = cpu->v[op->y] & 0x01;
    cpu->v[op->x] = cpu->v[op->y] >> 1;
    cpu->v[0xF] = bit > 0;
}

static void c8Subn(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u16 diff = (u16)cpu->v[op->y] - (u16)cpu->v[op->x];
    cpu->v[op->x] = (u8)(diff & 0x00FF);
    cpu->v[0xF] = diff <= 0x00FF;
}

static void c8Shl(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u8 bit = cpu->v[op->y] & 0x80;
    cpu->v[op->x] = cpu->v[op->y] << 1;
    cpu->v[0xF] = bit > 0;
}

static void c8Rnd(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_BYTE);
    cpu->v[op->x] = GetRandomValue(0, 255) & op->byte;
}

static void c8Drw(Chip8CPU *cpu, Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY_N);
    
    const u8 height = op->nibble;
    const u8 x0 = cpu->v[op->x] % C8_SCREEN_BUFFER_WIDTH;
    const u8 y0 = cpu->v[op->y] % C8_SCREEN_BUFFER_HEIGHT;

    cpu->v[0xF] = 0;
    for (u8 y = 0; y < height; y++)
    {
        const u16 y1 = (u16)(y0 + y);
        if (y1 >= C8_SCREEN_BUFFER_HEIGHT)
        {
            continue;
        }

        const u8 sprite = c8RAMRead(ram, cpu->idx + y);
        for (u8 x = 0; x < 8; x++)
        {
            const u16 x1 = (u16)(x0 + x);
            if (x1 >= C8_SCREEN_BUFFER_WIDTH)
            {
                continue;
            }

            const u8 spritePx = sprite & (0x80 >> x);
            if (spritePx > 0)
            {
                const u16 idx = x1 + y1 * C8_SCREEN_BUFFER_WIDTH;
                if (cpu->video[idx] == 0xFF)
                {
                    cpu->v[0xF] = 1;
                }

                cpu->video[idx] ^= 0xFF;
            }
        }
    }
}

static void c8Skp(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX);
    const u8 key = cpu->v[op->x];
    if (cpu->keypad[key])
    {
        cpu->pc += 2;
    }
}

static void c8Sknp(Chip8CPU *cpu, UNUSED Chip8RAM *ram, const Chip8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX);
    const u8 key = cpu->v[op->x];
    if (!cpu->keypad[key])
    {
        cpu->pc += 2;
    }
}
