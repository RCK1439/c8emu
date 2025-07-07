#include "cpu.h"
#include "debug.h"
#include "ram.h"
#include "instructions.h"
#include "stack.h"

#include "core/types.h"

#include <raylib.h>
#include <memory.h>

typedef void (*Chip8Exec)(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);

static void c8Raw(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Cls(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Ret(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Sys(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Jp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Call(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Se(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Sne(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Ld(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Add(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Or(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8And(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Xor(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Sub(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Shr(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Subn(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Shl(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Rnd(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Drw(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Skp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);
static void c8Sknp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op);

static Chip8Exec executors[] = {
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

    memset(cpu.video, 0x00, sizeof(cpu.video));
    cpu.stack = c8InitStack();
    memset(cpu.keypad, 0x00, sizeof(cpu.keypad));
    memset(cpu.v, 0x00, sizeof(cpu.v));
    cpu.idx = 0;
    cpu.pc = ADDR_PC;
    cpu.dt = 0;
    cpu.st = 0;

    return cpu;
}

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram)
{
    const u16 raw = (c8RAMRead(ram, cpu->pc) << 8) | c8RAMRead(ram, cpu->pc + 1);
    cpu->pc += 2;

    const OpCode opcode = DecodeOpCode(raw);
    executors[opcode.instr](cpu, ram, &opcode);

    if (cpu->dt > 0)
    {
        cpu->dt--;
    }

    if (cpu->st > 0)
    {
        cpu->st--;
    }
}

void c8SetCPUKey(Chip8CPU *cpu, u8 key, u8 val)
{
    cpu->keypad[key] = val;
}

void c8DrawCPUBuffer(const Chip8CPU *cpu)
{
    for (u16 y = 0; y < SCREEN_BUFFER_HEIGHT; y++)
    {
        for (u16 x = 0; x < SCREEN_BUFFER_WIDTH; x++)
        {
            if (cpu->video[x + y * SCREEN_BUFFER_WIDTH])
            {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }

    DRAW_DEBUG_INFO(cpu->v, cpu->dt, cpu->st, cpu->idx, cpu->pc, cpu->keypad);
}

static void c8Raw(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    (void)cpu;
    (void)ram;
    (void)op;
}

static void c8Cls(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    memset(cpu->video, 0x00, sizeof(cpu->video));
    
    (void)ram;
    (void)op;
}

static void c8Ret(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->pc = c8StackPop(&cpu->stack);

    (void)ram;
    (void)op;
}

static void c8Sys(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    (void)cpu;
    (void)ram;
    (void)op;
}

static void c8Jp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (op->addressMode == AM_ADDR)
    {
        cpu->pc = op->address;
    }
    else if (op->addressMode == AM_V0_ADDR)
    {
        cpu->pc = cpu->v[0x0] + op->address;
    }

    (void)ram;
}

static void c8Call(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    c8StackPush(&cpu->stack, cpu->pc);
    cpu->pc = op->address;

    (void)ram;
}

static void c8Se(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        if (cpu->v[op->x] == op->byte)
        {
            cpu->pc += 2;
        }
    }
    else if (op->addressMode == AM_VX_VY)
    {
        if (cpu->v[op->x] == cpu->v[op->y])
        {
            cpu->pc += 2;
        }
    }

    (void)ram;
}

static void c8Sne(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        if (cpu->v[op->x] != op->byte)
        {
            cpu->pc += 2;
        }
    }
    else if (op->addressMode == AM_VX_VY)
    {
        if (cpu->v[op->x] != cpu->v[op->y])
        {
            cpu->pc += 2;
        }
    }

    (void)ram;
}

static void c8Ld(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        cpu->v[op->x] = op->byte;
    }
    else if (op->addressMode == AM_VX_VY)
    {
        cpu->v[op->x] = cpu->v[op->y];
    }
    else if (op->addressMode == AM_I_ADDR)
    {
        cpu->idx = op->address;
    }
    else if (op->addressMode == AM_VX_DT)
    {
        cpu->v[op->x] = cpu->dt;
    }
    else if (op->addressMode == AM_VX_KEY)
    {
        Chip8Bool found = C8_FALSE;
        for (u8 i = 0; i < NUM_KEYS; i++)
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
    }
    else if (op->addressMode == AM_DT_VX)
    {
        cpu->dt = cpu->v[op->x];
    }
    else if (op->addressMode == AM_ST_VX)
    {
        cpu->st = cpu->v[op->x];
    }
    else if (op->addressMode == AM_FONT_VX)
    {
        const u8 digit = cpu->v[op->x];
        cpu->idx = ADDR_FONT + (5 * digit);
    }
    else if (op->addressMode == AM_BCD_VX)
    {
        u8 value = cpu->v[op->x];
        c8RAMWrite(ram, cpu->idx + 2, value % 10);
	    value /= 10;

        c8RAMWrite(ram, cpu->idx + 1, value % 10);
	    value /= 10;

        c8RAMWrite(ram, cpu->idx, value % 10);
    }
    else if (op->addressMode == AM_ADDR_I_VX)
    {
	    for (u8 i = 0; i <= op->x; i++)
        {
            c8RAMWrite(ram, cpu->idx + i, cpu->v[i]);
        }
    }
    else if (op->addressMode == AM_VX_ADDR_I)
    {
        for (u8 i = 0; i <= op->x; i++)
        {
            cpu->v[i] = c8RAMRead(ram, cpu->idx + i);
        }
    }
}

static void c8Add(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        cpu->v[op->x] += op->byte;
    }
    else if (op->addressMode == AM_VX_VY)
    {
        const u16 sum = (u16)cpu->v[op->x] + (u16)cpu->v[op->y];
        if (sum > 0x00FF)
        {
            cpu->v[0xF] = 1;
        }
        else
        {
            cpu->v[0xF] = 0;
        }

        cpu->v[op->x] = sum & 0x00FF;
    }
    else if (op->addressMode == AM_I_VX)
    {
        cpu->idx += cpu->v[op->x];
    }

    (void)ram;
}

static void c8Or(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[op->x] |= cpu->v[op->y];

    (void)ram;
}

static void c8And(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[op->x] &= cpu->v[op->y];

    (void)ram;
}

static void c8Xor(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[op->x] ^= cpu->v[op->y];
    
    (void)ram;
}

static void c8Sub(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (cpu->v[op->x] > cpu->v[op->y])
    {
        cpu->v[0xF] = 1;
    }
    else
    {
        cpu->v[0xF] = 0;
    }

    cpu->v[op->x] -= cpu->v[op->y];

    (void)ram;
}

static void c8Shr(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[0xF] = cpu->v[op->x] & 0x01;
    cpu->v[op->x] >>= 1;

    (void)ram;
}

static void c8Subn(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    if (cpu->v[op->y] > cpu->v[op->x])
    {
        cpu->v[0xF] = 1;
    }
    else
    {
        cpu->v[0xF] = 0;
    }
    cpu->v[op->x] = cpu->v[op->y] - cpu->v[op->x];
    
    (void)ram;
}

static void c8Shl(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[0xF] = (cpu->v[op->x] & 0x80) >> 7;
    cpu->v[op->x] <<= 1;

    (void)ram;
}

static void c8Rnd(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    cpu->v[op->x] = GetRandomValue(0, 255) & op->byte;

    (void)ram;
}

static void c8Drw(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    const u8 height = op->nibble;
    const u8 xp = cpu->v[op->x] % SCREEN_BUFFER_WIDTH;
    const u8 yp = cpu->v[op->y] % SCREEN_BUFFER_HEIGHT;

    cpu->v[0xF] = 0;
    for (u8 r = 0; r < height; r++)
    {
        const u8 sprite = c8RAMRead(ram, cpu->idx + r);
        for (u8 c = 0; c < 8; c++)
        {
            const u8 spritePx = sprite & (0x80 >> c);
            u8 *const screenPx = &cpu->video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (spritePx)
            {
                if (*screenPx == 0xFF)
                {
                    cpu->v[0xF] = 1;
                }

                *screenPx ^= 0xFF;
            }
        }
    }
}

static void c8Skp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    const u8 key = cpu->v[op->x];
    if (cpu->keypad[key])
    {
        cpu->pc += 2;
    }

    (void)ram;
}

static void c8Sknp(Chip8CPU *cpu, Chip8RAM *ram, const OpCode *op)
{
    const u8 key = cpu->v[op->x];
    if (!cpu->keypad[key])
    {
        cpu->pc += 2;
    }

    (void)ram;
}
