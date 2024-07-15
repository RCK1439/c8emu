/**
 * @file   cpu.c
 * @brief  Implementation of the Chip-8 execution instructions.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#include "cpu.h"
#include "constants.h"
#include "debug.h"
#include "instructions.h"
#include "ram.h"
#include "stack.h"

#include <raylib.h>
#include <assert.h>
#include <stdbool.h>
#include <memory.h>
#include <stdint.h>

#define unused __attribute__((unused))

/* --- type definitions ---------------------------------------------------- */

/**
 * This struct defines the context for the processor. i.e. The registers,
 * stack, video buffer and keypad.
 */
typedef struct CPUContext {
    uint8_t v[NUM_REGISTERS];
    
    uint16_t idx;
    uint16_t pc;

    uint8_t dt;
    uint8_t st;

    Stack stack;
    uint8_t video[SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT];
    uint8_t keypad[NUM_KEYS];
} CPUContext;

/**
 * Defines a function signature for an executor routine.
 */
typedef void (*ExecutorFunction)(const OpCode *const op);

/* --- executor routines --------------------------------------------------- */

static void ExecRAW(const OpCode *const op);
static void ExecCLS(const OpCode *const op);
static void ExecRET(const OpCode *const op);
static void ExecSYS(const OpCode *const op);
static void ExecJP(const OpCode *const op);
static void ExecCALL(const OpCode *const op);
static void ExecSE(const OpCode *const op);
static void ExecSNE(const OpCode *const op);
static void ExecLD(const OpCode *const op);
static void ExecADD(const OpCode *const op);
static void ExecOR(const OpCode *const op);
static void ExecAND(const OpCode *const op);
static void ExecXOR(const OpCode *const op);
static void ExecSUB(const OpCode *const op);
static void ExecSHR(const OpCode *const op);
static void ExecSUBN(const OpCode *const op);
static void ExecSHL(const OpCode *const op);
static void ExecRND(const OpCode *const op);
static void ExecDRW(const OpCode *const op);
static void ExecSKP(const OpCode *const op);
static void ExecSKNP(const OpCode *const op);

/* --- global variables ---------------------------------------------------- */

static CPUContext s_CPU;
static ExecutorFunction s_Executors[] = {
    [IN_RAW] = ExecRAW,
    [IN_CLS] = ExecCLS,
    [IN_RET] = ExecRET,
    [IN_SYS] = ExecSYS,
    [IN_JP] = ExecJP,
    [IN_CALL] = ExecCALL,
    [IN_SE] = ExecSE,
    [IN_SNE] = ExecSNE,
    [IN_LD] = ExecLD,
    [IN_ADD] = ExecADD,
    [IN_OR] = ExecOR,
    [IN_AND] = ExecAND,
    [IN_XOR] = ExecXOR,
    [IN_SUB] = ExecSUB,
    [IN_SHR] = ExecSHR,
    [IN_SUBN] = ExecSUBN,
    [IN_SHL] = ExecSHL,
    [IN_RND] = ExecRND,
    [IN_DRW] = ExecDRW,
    [IN_SKP] = ExecSKP,
    [IN_SKNP] = ExecSKNP
};

/* --- cpu interface ------------------------------------------------------- */

void InitCPU(void)
{
    memset(s_CPU.v, 0x00, sizeof(s_CPU.v));
    s_CPU.idx = 0;
    s_CPU.pc = ADDR_PC;
    s_CPU.dt = 0;
    s_CPU.st = 0;
    s_CPU.stack = InitStack();
    memset(s_CPU.video, 0x00, sizeof(s_CPU.video));
    memset(s_CPU.keypad, 0x00, sizeof(s_CPU.keypad));
}

void CPUStep(void)
{
    const uint16_t raw_op = (RAMRead(s_CPU.pc) << 8) | RAMRead(s_CPU.pc + 1);
    s_CPU.pc += 2;

    const OpCode opcode = DecodeOpCode(raw_op);
    s_Executors[opcode.instr](&opcode);

    if (s_CPU.dt > 0)
        s_CPU.dt--;

    if (s_CPU.st > 0)
        s_CPU.st--;
}

void CPUSetKey(uint8_t key, uint8_t val)
{
    s_CPU.keypad[key] = val;
}

void CPUDrawBuffer(void)
{
    for (uint16_t y = 0; y < SCREEN_BUFFER_HEIGHT; y++)
        for (uint16_t x = 0; x < SCREEN_BUFFER_WIDTH; x++)
            if (s_CPU.video[x + y * SCREEN_BUFFER_WIDTH])
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);

    DRAW_DEBUG_INFO(s_CPU.v, s_CPU.dt, s_CPU.st, s_CPU.idx, s_CPU.pc, s_CPU.keypad);
}

/* --- executor routines --------------------------------------------------- */

static void ExecRAW(unused const OpCode *const op)
{
    /* Intentionally left empty */
}

static void ExecCLS(unused const OpCode *const op)
{
    memset(s_CPU.video, 0x00, sizeof(s_CPU.video));
}

static void ExecRET(unused const OpCode *const op)
{
    s_CPU.pc = StackPop(&s_CPU.stack);    
}

static void ExecSYS(unused const OpCode *const op)
{
    /* Intentionally left empty */
}

static void ExecJP(const OpCode *const op)
{
    if (op->addr_mode == AM_ADDR)
    {
        s_CPU.pc = op->address;
    }
    else if (op->addr_mode == AM_V0_ADDR)
    {
        s_CPU.pc = s_CPU.v[0x0] + op->address;
    }
}

static void ExecCALL(const OpCode *const op)
{
    StackPush(&s_CPU.stack, s_CPU.pc);
    s_CPU.pc = op->address;
}

static void ExecSE(const OpCode *const op)
{
    if (op->addr_mode == AM_VX_BYTE)
    {
        if (s_CPU.v[op->x_reg] == op->byte)
            s_CPU.pc += 2;
    }
    else if (op->addr_mode == AM_VX_VY)
    {
        if (s_CPU.v[op->x_reg] == s_CPU.v[op->y_reg])
            s_CPU.pc += 2;
    }
}

static void ExecSNE(const OpCode *const op)
{
    if (op->addr_mode == AM_VX_BYTE)
    {
        if (s_CPU.v[op->x_reg] != op->byte)
            s_CPU.pc += 2;
    }
    else if (op->addr_mode == AM_VX_VY)
    {
        if (s_CPU.v[op->x_reg] != s_CPU.v[op->y_reg])
            s_CPU.pc += 2;
    }
}

static void ExecLD(const OpCode *const op)
{
    if (op->addr_mode == AM_VX_BYTE)
    {
        s_CPU.v[op->x_reg] = op->byte;
    }
    else if (op->addr_mode == AM_VX_VY)
    {
        s_CPU.v[op->x_reg] = s_CPU.v[op->y_reg];
    }
    else if (op->addr_mode == AM_I_ADDR)
    {
        s_CPU.idx = op->address;
    }
    else if (op->addr_mode == AM_VX_DT)
    {
        s_CPU.v[op->x_reg] = s_CPU.dt;
    }
    else if (op->addr_mode == AM_VX_KEY)
    {    
        bool found = false;
        for (uint8_t i = 0; i < NUM_KEYS; i++)
        {
            if (s_CPU.keypad[i]) 
            {
                found = true;
                s_CPU.v[op->x_reg] = i;
                break;
            }
        }

        if (!found)
            s_CPU.pc -= 2;
    }
    else if (op->addr_mode == AM_DT_VX)
    {
        s_CPU.dt = s_CPU.v[op->x_reg];
    }
    else if (op->addr_mode == AM_ST_VX)
    {
        s_CPU.st = s_CPU.v[op->x_reg];
    }
    else if (op->addr_mode == AM_FONT_VX)
    {
        const uint8_t digit = s_CPU.v[op->x_reg];
        s_CPU.idx = ADDR_FONT + (5 * digit);
    }
    else if (op->addr_mode == AM_BCD_VX)
    {
	    uint8_t value;

        value = s_CPU.v[op->x_reg];
        RAMWrite(s_CPU.idx + 2, value % 10);
	    value /= 10;

        RAMWrite(s_CPU.idx + 1, value % 10);
	    value /= 10;

        RAMWrite(s_CPU.idx, value % 10);
    }
    else if (op->addr_mode == AM_ADDR_I_VX)
    {
	    for (uint8_t i = 0; i <= op->x_reg; ++i)
            RAMWrite(s_CPU.idx + i, s_CPU.v[i]);
    }
    else if (op->addr_mode == AM_VX_ADDR_I)
    {
        for (uint8_t i = 0; i <= op->x_reg; i++)
            s_CPU.v[i] = RAMRead(s_CPU.idx + i);
    }
}

static void ExecADD(const OpCode *const op)
{
    if (op->addr_mode == AM_VX_BYTE)
    {
        s_CPU.v[op->x_reg] += op->byte;
    }
    else if (op->addr_mode == AM_VX_VY)
    {
        const uint16_t sum = s_CPU.v[op->x_reg] + s_CPU.v[op->y_reg];
        if (sum > 0x00FF)
        {
            s_CPU.v[0xF] = 1;
        }
        else
        {
            s_CPU.v[0xF] = 0;
        }

        s_CPU.v[op->x_reg] = sum & 0x00FF;
    }
    else if (op->addr_mode == AM_I_VX)
    {
        s_CPU.idx += s_CPU.v[op->x_reg];
    }
}

static void ExecOR(const OpCode *const op)
{
    s_CPU.v[op->x_reg] |= s_CPU.v[op->y_reg];
}

static void ExecAND(const OpCode *const op)
{
    s_CPU.v[op->x_reg] &= s_CPU.v[op->y_reg];
}

static void ExecXOR(const OpCode *const op)
{
    s_CPU.v[op->x_reg] ^= s_CPU.v[op->y_reg];
}

static void ExecSUB(const OpCode *const op)
{
    if (s_CPU.v[op->x_reg] > s_CPU.v[op->y_reg])
    {
        s_CPU.v[0xF] = 1;
    }
    else
    {
        s_CPU.v[0xF] = 0;
    }

    s_CPU.v[op->x_reg] -= s_CPU.v[op->y_reg];
}

static void ExecSHR(const OpCode *const op)
{
    s_CPU.v[0xF] = s_CPU.v[op->x_reg] & 0x01;
    s_CPU.v[op->x_reg] >>= 1;
}

static void ExecSUBN(const OpCode *const op)
{
    if (s_CPU.v[op->y_reg] > s_CPU.v[op->x_reg])
    {
        s_CPU.v[0xF] = 1;
    }
    else
    {
        s_CPU.v[0xF] = 0;
    }

    s_CPU.v[op->x_reg] = s_CPU.v[op->y_reg] - s_CPU.v[op->x_reg];
}

static void ExecSHL(const OpCode *const op)
{
    s_CPU.v[0xF] = (s_CPU.v[op->x_reg] & 0x80) >> 7;
    s_CPU.v[op->x_reg] <<= 1;
}

static void ExecRND(const OpCode *const op)
{
    s_CPU.v[op->x_reg] = GetRandomValue(0, 255) & op->byte;
}

static void ExecDRW(const OpCode *const op)
{
    const uint8_t height = op->nibble;
    const uint8_t xp = s_CPU.v[op->x_reg] % SCREEN_BUFFER_WIDTH;
    const uint8_t yp = s_CPU.v[op->y_reg] % SCREEN_BUFFER_HEIGHT;

    s_CPU.v[0xF] = 0;
    for (uint8_t r = 0; r < height; r++)
    {
        const uint8_t sprite = RAMRead(s_CPU.idx + r);
        for (uint8_t c = 0; c < 8; c++)
        {
            const uint8_t sprite_px = sprite & (0x80 >> c);
            uint8_t *const screen_px = &s_CPU.video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (sprite_px)
            {
                if (*screen_px == 0xFF)
                    s_CPU.v[0xF] = 1;

                *screen_px ^= 0xFF;
            }
        }
    }
}

static void ExecSKP(const OpCode *const op)
{
    const uint8_t key = s_CPU.v[op->x_reg];
    if (s_CPU.keypad[key])
        s_CPU.pc += 2;
}

static void ExecSKNP(const OpCode *const op)
{
    const uint8_t key = s_CPU.v[op->x_reg];
    if (!s_CPU.keypad[key])
        s_CPU.pc += 2;
}

