#include "cpu.h"
#include "debug.h"
#include "instructions.h"
#include "ram.h"
#include "stack.h"

#include <raylib.h>
#include <stdbool.h>
#include <memory.h>
#include <stdint.h>

#define NUM_REGISTERS 16
#define NUM_KEYS 16

#define SCREEN_BUFFER_WIDTH 64
#define SCREEN_BUFFER_HEIGHT 32
#define SCALE 16

#define unused __attribute__((unused))

typedef struct CPU
{
    uint8_t   v[NUM_REGISTERS];
    
    uint16_t  idx;
    uint16_t  pc;

    uint8_t   dt;
    uint8_t   st;

    CallStack stack;
    uint8_t   video[SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT];
    uint8_t   keypad[NUM_KEYS];
} CPU;

typedef void (*Executor)(const OpCode *const op);

static void Raw(const OpCode *const op);
static void Cls(const OpCode *const op);
static void Ret(const OpCode *const op);
static void Sys(const OpCode *const op);
static void Jp(const OpCode *const op);
static void Call(const OpCode *const op);
static void Se(const OpCode *const op);
static void Sne(const OpCode *const op);
static void Ld(const OpCode *const op);
static void Add(const OpCode *const op);
static void Or(const OpCode *const op);
static void And(const OpCode *const op);
static void Xor(const OpCode *const op);
static void Sub(const OpCode *const op);
static void Shr(const OpCode *const op);
static void Subn(const OpCode *const op);
static void Shl(const OpCode *const op);
static void Rnd(const OpCode *const op);
static void Drw(const OpCode *const op);
static void Skp(const OpCode *const op);
static void Sknp(const OpCode *const op);

static CPU cpu;
static Executor executors[] = {
    [IN_RAW] = Raw,
    [IN_CLS] = Cls,
    [IN_RET] = Ret,
    [IN_SYS] = Sys,
    [IN_JP] = Jp,
    [IN_CALL] = Call,
    [IN_SE] = Se,
    [IN_SNE] = Sne,
    [IN_LD] = Ld,
    [IN_ADD] = Add,
    [IN_OR] = Or,
    [IN_AND] = And,
    [IN_XOR] = Xor,
    [IN_SUB] = Sub,
    [IN_SHR] = Shr,
    [IN_SUBN] = Subn,
    [IN_SHL] = Shl,
    [IN_RND] = Rnd,
    [IN_DRW] = Drw,
    [IN_SKP] = Skp,
    [IN_SKNP] = Sknp
};

void InitCPU(void)
{
    memset(cpu.v, 0x00, sizeof(cpu.v));
    cpu.idx = 0;
    cpu.pc = ADDR_PC;
    cpu.dt = 0;
    cpu.st = 0;
    cpu.stack = CreateStack();
    memset(cpu.video, 0x00, sizeof(cpu.video));
    memset(cpu.keypad, 0x00, sizeof(cpu.keypad));
}

void StepCPU(void)
{
    const uint16_t raw = (RAMRead(cpu.pc) << 8) | RAMRead(cpu.pc + 1);
    cpu.pc += 2;

    const OpCode opcode = DecodeOpCode(raw);
    executors[opcode.instr](&opcode);

    if (cpu.dt > 0)
    {
        cpu.dt--;
    }

    if (cpu.st > 0)
    {
        cpu.st--;
    }
}

void SetCPUKey(uint8_t key, uint8_t val)
{
    cpu.keypad[key] = val;
}

void DrawCPUBuffer(void)
{
    for (uint16_t y = 0; y < SCREEN_BUFFER_HEIGHT; y++)
    {
        for (uint16_t x = 0; x < SCREEN_BUFFER_WIDTH; x++)
        {
            if (cpu.video[x + y * SCREEN_BUFFER_WIDTH])
            {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }

    DRAW_DEBUG_INFO(cpu.v, cpu.dt, cpu.st, cpu.idx, cpu.pc, cpu.keypad);
}

static void Raw(unused const OpCode *const op)
{
    /* Intentionally left empty */
}

static void Cls(unused const OpCode *const op)
{
    memset(cpu.video, 0x00, sizeof(cpu.video));
}

static void Ret(unused const OpCode *const op)
{
    cpu.pc = StackPop(&cpu.stack);    
}

static void Sys(unused const OpCode *const op)
{
    /* Intentionally left empty */
}

static void Jp(const OpCode *const op)
{
    if (op->addressMode == AM_ADDR)
    {
        cpu.pc = op->address;
    }
    else if (op->addressMode == AM_V0_ADDR)
    {
        cpu.pc = cpu.v[0x0] + op->address;
    }
}

static void Call(const OpCode *const op)
{
    StackPush(&cpu.stack, cpu.pc);
    cpu.pc = op->address;
}

static void Se(const OpCode *const op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        if (cpu.v[op->x] == op->byte)
        {
            cpu.pc += 2;
        }
    }
    else if (op->addressMode == AM_VX_VY)
    {
        if (cpu.v[op->x] == cpu.v[op->y])
        {
            cpu.pc += 2;
        }
    }
}

static void Sne(const OpCode *const op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        if (cpu.v[op->x] != op->byte)
        {
            cpu.pc += 2;
        }
    }
    else if (op->addressMode == AM_VX_VY)
    {
        if (cpu.v[op->x] != cpu.v[op->y])
        {
            cpu.pc += 2;
        }
    }
}

static void Ld(const OpCode *const op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        cpu.v[op->x] = op->byte;
    }
    else if (op->addressMode == AM_VX_VY)
    {
        cpu.v[op->x] = cpu.v[op->y];
    }
    else if (op->addressMode == AM_I_ADDR)
    {
        cpu.idx = op->address;
    }
    else if (op->addressMode == AM_VX_DT)
    {
        cpu.v[op->x] = cpu.dt;
    }
    else if (op->addressMode == AM_VX_KEY)
    {
        bool found = false;
        for (uint8_t i = 0; i < NUM_KEYS; i++)
        {
            if (cpu.keypad[i])
            {
                found = true;
                cpu.v[op->x] = i;
                break;
            }
        }

        if (!found)
        {
            cpu.pc -= 2;
        }
    }
    else if (op->addressMode == AM_DT_VX)
    {
        cpu.dt = cpu.v[op->x];
    }
    else if (op->addressMode == AM_ST_VX)
    {
        cpu.st = cpu.v[op->x];
    }
    else if (op->addressMode == AM_FONT_VX)
    {
        const uint8_t digit = cpu.v[op->x];
        cpu.idx = ADDR_FONT + (5 * digit);
    }
    else if (op->addressMode == AM_BCD_VX)
    {
        uint8_t value = cpu.v[op->x];
        RAMWrite(cpu.idx + 2, value % 10);
	    value /= 10;

        RAMWrite(cpu.idx + 1, value % 10);
	    value /= 10;

        RAMWrite(cpu.idx, value % 10);
    }
    else if (op->addressMode == AM_ADDR_I_VX)
    {
	    for (uint8_t i = 0; i <= op->x; i++)
        {
            RAMWrite(cpu.idx + i, cpu.v[i]);
        }
    }
    else if (op->addressMode == AM_VX_ADDR_I)
    {
        for (uint8_t i = 0; i <= op->x; i++)
        {
            cpu.v[i] = RAMRead(cpu.idx + i);
        }
    }
}

static void Add(const OpCode *const op)
{
    if (op->addressMode == AM_VX_BYTE)
    {
        cpu.v[op->x] += op->byte;
    }
    else if (op->addressMode == AM_VX_VY)
    {
        const uint16_t sum = cpu.v[op->x] + cpu.v[op->y];
        if (sum > 0x00FF)
        {
            cpu.v[0xF] = 1;
        }
        else
        {
            cpu.v[0xF] = 0;
        }

        cpu.v[op->x] = sum & 0x00FF;
    }
    else if (op->addressMode == AM_I_VX)
    {
        cpu.idx += cpu.v[op->x];
    }
}

static void Or(const OpCode *const op)
{
    cpu.v[op->x] |= cpu.v[op->y];
}

static void And(const OpCode *const op)
{
    cpu.v[op->x] &= cpu.v[op->y];
}

static void Xor(const OpCode *const op)
{
    cpu.v[op->x] ^= cpu.v[op->y];
}

static void Sub(const OpCode *const op)
{
    if (cpu.v[op->x] > cpu.v[op->y])
    {
        cpu.v[0xF] = 1;
    }
    else
    {
        cpu.v[0xF] = 0;
    }

    cpu.v[op->x] -= cpu.v[op->y];
}

static void Shr(const OpCode *const op)
{
    cpu.v[0xF] = cpu.v[op->x] & 0x01;
    cpu.v[op->x] >>= 1;
}

static void Subn(const OpCode *const op)
{
    if (cpu.v[op->y] > cpu.v[op->x])
    {
        cpu.v[0xF] = 1;
    }
    else
    {
        cpu.v[0xF] = 0;
    }

    cpu.v[op->x] = cpu.v[op->y] - cpu.v[op->x];
}

static void Shl(const OpCode *const op)
{
    cpu.v[0xF] = (cpu.v[op->x] & 0x80) >> 7;
    cpu.v[op->x] <<= 1;
}

static void Rnd(const OpCode *const op)
{
    cpu.v[op->x] = GetRandomValue(0, 255) & op->byte;
}

static void Drw(const OpCode *const op)
{
    const uint8_t height = op->nibble;
    const uint8_t xp = cpu.v[op->x] % SCREEN_BUFFER_WIDTH;
    const uint8_t yp = cpu.v[op->y] % SCREEN_BUFFER_HEIGHT;

    cpu.v[0xF] = 0;
    for (uint8_t r = 0; r < height; r++)
    {
        const uint8_t sprite = RAMRead(cpu.idx + r);
        for (uint8_t c = 0; c < 8; c++)
        {
            const uint8_t spritePx = sprite & (0x80 >> c);
            uint8_t *const screenPx = &cpu.video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (spritePx)
            {
                if (*screenPx == 0xFF)
                {
                    cpu.v[0xF] = 1;
                }

                *screenPx ^= 0xFF;
            }
        }
    }
}

static void Skp(const OpCode *const op)
{
    const uint8_t key = cpu.v[op->x];
    if (cpu.keypad[key])
    {
        cpu.pc += 2;
    }
}

static void Sknp(const OpCode *const op)
{
    const uint8_t key = cpu.v[op->x];
    if (!cpu.keypad[key])
    {
        cpu.pc += 2;
    }
}
