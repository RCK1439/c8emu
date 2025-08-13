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

// --- debug macros -----------------------------------------------------------

#if defined(C8_DEBUG)
#define C8_ENSURE_ADDR_MODE(addr_mode, expected)                 \
    if ((addr_mode) != (expected))                               \
        c8Panic(ERR_INVALID_ADDRESS_MODE,                        \
            "Unexpected address mode in %s executor routine: %d",\
            __func__,                                            \
            (addr_mode))
#else
#define C8_ENSURE_ADDR_MODE(addr_mode, expected)
#endif

// --- type definitions -------------------------------------------------------

/**
 * Function pointer type for executing Chip-8 instructions
 */
typedef void (*C8ExecProc)(C8CPU*, C8RAM*, const C8OpCode*);

// --- instruction executors --------------------------------------------------

static void c8Raw(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Cls(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Ret(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Jp(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Call(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Se(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Sne(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Ld(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Add(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Or(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8And(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Xor(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Sub(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Shr(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Subn(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Shl(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Rnd(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Drw(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Skp(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);
static void c8Sknp(C8CPU *cpu, C8RAM *ram, const C8OpCode *op);

static const C8ExecProc s_Executors[] = {
    [IN_RAW] = c8Raw,
    [IN_CLS] = c8Cls,
    [IN_RET] = c8Ret,
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

// --- cpu interface ----------------------------------------------------------

C8CPU c8InitCPU(void)
{
    C8CPU cpu = { 0 };
    cpu.stack = c8InitStack();
    cpu.pc = C8_ADDR_PC;

    return cpu;
}

void c8StepCPU(C8CPU *cpu, C8RAM *ram)
{
    for (u8 i = 0; i < C8_OPS_PER_CYCLE; i++)
    {
        const u16 raw = ((u16)c8RAMRead(ram, cpu->pc) << 8) | (u16)c8RAMRead(ram, cpu->pc + 1);
        cpu->pc += 2;

        const C8OpCode opcode = c8DecodeOpCode(raw);
        s_Executors[opcode.instr](cpu, ram, &opcode);
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

void c8SetCPUKey(C8CPU *cpu, C8Key key, u8 val)
{
    cpu->keypad[key] = val;
}

// --- executor implementations -----------------------------------------------

static void c8Raw(UNUSED C8CPU *cpu, UNUSED C8RAM *ram, UNUSED const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_OPCODE);
    /* Intentionally left empty */
}

static void c8Cls(C8CPU *cpu, UNUSED C8RAM *ram, UNUSED const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_NONE);
    memset(cpu->video, 0x00, sizeof(cpu->video));
}

static void c8Ret(C8CPU *cpu, UNUSED C8RAM *ram, UNUSED const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_NONE);
    cpu->pc = c8PopAddr(&cpu->stack);
}

static void c8Jp(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_ADDR:
            cpu->pc = op->address;
            break;
        case AM_V0_ADDR:
            cpu->pc = cpu->registers[V0] + op->address;
            break;
        default:
            c8Panic(ERR_INVALID_ADDRESS_MODE, "Unexpected address mode in %s executor routine: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Call(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_ADDR);
    c8PushAddr(&cpu->stack, cpu->pc);
    cpu->pc = op->address;
}

static void c8Se(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            if (cpu->registers[op->x] == op->byte)
            {
                cpu->pc += 2;
            }
            break;
        case AM_VX_VY:
            if (cpu->registers[op->x] == cpu->registers[op->y])
            {
                cpu->pc += 2;
            }
            break;
        default:
            c8Panic(ERR_INVALID_ADDRESS_MODE, "Unexpected address mode in %s executor routine: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Sne(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            if (cpu->registers[op->x] != op->byte)
            {
                cpu->pc += 2;
            }
            break;
        case AM_VX_VY:
            if (cpu->registers[op->x] != cpu->registers[op->y])
            {
                cpu->pc += 2;
            }
            break;
        default:
            c8Panic(ERR_INVALID_ADDRESS_MODE, "Unexpected address mode in %s executor routine: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Ld(C8CPU *cpu, C8RAM *ram, const C8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
            cpu->registers[op->x] = op->byte;
            break;
        case AM_VX_VY:
            cpu->registers[op->x] = cpu->registers[op->y];
            break;
        case AM_I_ADDR:
            cpu->idx = op->address;
            break;
        case AM_VX_DT:
            cpu->registers[op->x] = cpu->dt;
            break;
        case AM_VX_KEY:
        {
            C8Bool found = C8_FALSE;
            for (u8 i = 0; i < C8_NUM_KEYS; i++)
            {
                if (cpu->keypad[i])
                {
                    found = C8_TRUE;
                    cpu->registers[op->x] = i;
                    break;
                }
            }

            if (!found)
            {
                cpu->pc -= 2;
            }
        } break;
        case AM_DT_VX:
            cpu->dt = cpu->registers[op->x];
            break;
        case AM_ST_VX:
            cpu->st = cpu->registers[op->x];
            break;
        case AM_FONT_VX:
        {
            const u8 digit = cpu->registers[op->x];
            cpu->idx = C8_ADDR_FONT + (5 * digit);
        } break;
        case AM_BCD_VX:
        {
            u8 value = cpu->registers[op->x];
            c8RAMWrite(ram, cpu->idx + 2, value % 10);
            value /= 10;

            c8RAMWrite(ram, cpu->idx + 1, value % 10);
            value /= 10;

            c8RAMWrite(ram, cpu->idx, value % 10);
        } break;
        case AM_ADDR_I_VX:
            for (u8 i = 0; i <= op->x; i++)
            {
                c8RAMWrite(ram, cpu->idx++, cpu->registers[i]);
            }
            break;
        case AM_VX_ADDR_I:
            for (u8 i = 0; i <= op->x; i++)
            {
                cpu->registers[i] = c8RAMRead(ram, cpu->idx++);
            }
            break;
        default:
            c8Panic(ERR_INVALID_ADDRESS_MODE, "Unexpected address mode in %s executor routine: %d", __func__, op->addressMode);
            break;
    }
}

static void c8Add(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    switch (op->addressMode)
    {
        case AM_VX_BYTE:
        {
            const u16 sum = (u16)cpu->registers[op->x] + (u16)op->byte;
            cpu->registers[op->x] = (u8)(sum & 0x00FF);
            cpu->registers[VF] = sum > 0x00FF;
        } break;
        case AM_VX_VY:
        {
            const u16 sum = (u16)cpu->registers[op->x] + (u16)cpu->registers[op->y];
            cpu->registers[op->x] = (u8)(sum & 0x00FF);
            cpu->registers[VF] = sum > 0x00FF;
        } break;
        case AM_I_VX:
            cpu->idx += cpu->registers[op->x];
            break;
        default:
            c8Panic(ERR_INVALID_ADDRESS_MODE, "Unexpected address mode in %s executor routine: %d", __func__, op->addressMode);
            break;    
    }
}

static void c8Or(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->registers[op->x] |= cpu->registers[op->y];
    cpu->registers[VF] = 0;
}

static void c8And(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->registers[op->x] &= cpu->registers[op->y];
    cpu->registers[VF] = 0;
}

static void c8Xor(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    cpu->registers[op->x] ^= cpu->registers[op->y];
    cpu->registers[VF] = 0;
}

static void c8Sub(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u16 diff = (u16)cpu->registers[op->x] - (u16)cpu->registers[op->y];
    cpu->registers[op->x] = (u8)(diff & 0x00FF);
    cpu->registers[VF] = diff <= 0x00FF;
}

static void c8Shr(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u8 bit = cpu->registers[op->y] & 0x01;
    cpu->registers[op->x] = cpu->registers[op->y] >> 1;
    cpu->registers[VF] = bit > 0;
}

static void c8Subn(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u16 diff = (u16)cpu->registers[op->y] - (u16)cpu->registers[op->x];
    cpu->registers[op->x] = (u8)(diff & 0x00FF);
    cpu->registers[VF] = diff <= 0x00FF;
}

static void c8Shl(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY);
    const u8 bit = cpu->registers[op->y] & 0x80;
    cpu->registers[op->x] = cpu->registers[op->y] << 1;
    cpu->registers[VF] = bit > 0;
}

static void c8Rnd(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_BYTE);
    cpu->registers[op->x] = GetRandomValue(0, 255) & op->byte;
}

static void c8Drw(C8CPU *cpu, C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX_VY_N);
    
    const u8 height = op->nibble;
    const u8 x0 = cpu->registers[op->x] % C8_SCREEN_BUFFER_WIDTH;
    const u8 y0 = cpu->registers[op->y] % C8_SCREEN_BUFFER_HEIGHT;

    cpu->registers[VF] = 0;
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
                    cpu->registers[VF] = 1;
                }

                cpu->video[idx] ^= 0xFF;
            }
        }
    }
}

static void c8Skp(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX);
    const u8 key = cpu->registers[op->x];
    if (cpu->keypad[key])
    {
        cpu->pc += 2;
    }
}

static void c8Sknp(C8CPU *cpu, UNUSED C8RAM *ram, const C8OpCode *op)
{
    C8_ENSURE_ADDR_MODE(op->addressMode, AM_VX);
    const u8 key = cpu->registers[op->x];
    if (!cpu->keypad[key])
    {
        cpu->pc += 2;
    }
}
