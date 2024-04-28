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
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

/* --- type definitions ---------------------------------------------------- */

/**
 * This struct defines the context for the processor. i.e. The registers,
 * stack, video buffer and keypad.
 */
typedef struct cpu_context_s {
    uint8_t v[NUM_REGISTERS];
    
    uint16_t idx;
    uint16_t pc;

    uint8_t dt;
    uint8_t st;

    stack_t stack;
    uint8_t video[SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT];
    uint8_t keypad[NUM_KEYS];
} cpu_context_t;

/**
 * Defines a function signature for an executor routine.
 */
typedef void (*exec_t)(opcode_t *op);

/* --- executor routines --------------------------------------------------- */

static void exec_raw(opcode_t *op);
static void exec_cls(opcode_t *op);
static void exec_ret(opcode_t *op);
static void exec_sys(opcode_t *op);
static void exec_jp(opcode_t *op);
static void exec_call(opcode_t *op);
static void exec_se(opcode_t *op);
static void exec_sne(opcode_t *op);
static void exec_ld(opcode_t *op);
static void exec_add(opcode_t *op);
static void exec_or(opcode_t *op);
static void exec_and(opcode_t *op);
static void exec_xor(opcode_t *op);
static void exec_sub(opcode_t *op);
static void exec_shr(opcode_t *op);
static void exec_subn(opcode_t *op);
static void exec_shl(opcode_t *op);
static void exec_rnd(opcode_t *op);
static void exec_drw(opcode_t *op);
static void exec_skp(opcode_t *op);
static void exec_sknp(opcode_t *op);

/* --- function prototypes ------------------------------------------------- */

#ifndef NDEBUG
/**
 * Draws debugging information about the CPU to the screen.
 */
static void draw_debug_info(void);
#endif

/* --- global variables ---------------------------------------------------- */

static cpu_context_t ctx;
static exec_t instr_executors[] = {
    [IN_RAW] = exec_raw,
    [IN_CLS] = exec_cls,
    [IN_RET] = exec_ret,
    [IN_SYS] = exec_sys,
    [IN_JP] = exec_jp,
    [IN_CALL] = exec_call,
    [IN_SE] = exec_se,
    [IN_SNE] = exec_sne,
    [IN_LD] = exec_ld,
    [IN_ADD] = exec_add,
    [IN_OR] = exec_or,
    [IN_AND] = exec_and,
    [IN_XOR] = exec_xor,
    [IN_SUB] = exec_sub,
    [IN_SHR] = exec_shr,
    [IN_SUBN] = exec_subn,
    [IN_SHL] = exec_shl,
    [IN_RND] = exec_rnd,
    [IN_DRW] = exec_drw,
    [IN_SKP] = exec_skp,
    [IN_SKNP] = exec_sknp
};

/* --- cpu interface ------------------------------------------------------- */

void cpu_init(void)
{
    memset(ctx.v, 0x00, sizeof(ctx.v));
    ctx.idx = 0;
    ctx.pc = ADDR_PC;
    ctx.dt = 0;
    ctx.st = 0;
    ctx.stack = stack_init();
    memset(ctx.video, 0x00, sizeof(ctx.video));
    memset(ctx.keypad, 0x00, sizeof(ctx.keypad));
}

void cpu_step(void)
{
    uint16_t raw_op;
    opcode_t opcode;

    raw_op = (ram_read(ctx.pc) << 8) | ram_read(ctx.pc + 1);
    ctx.pc += 2;

    opcode = decode_opcode(raw_op);
    instr_executors[opcode.instr](&opcode);

    if (ctx.dt > 0) {
        ctx.dt--;
    }

    if (ctx.st > 0) {
        ctx.st--;
    }
}

void cpu_setkey(uint8_t key, uint8_t val)
{
    ctx.keypad[key] = val;
}

void cpu_draw_buffer(void)
{
    uint16_t x, y;

    for (y = 0; y < SCREEN_BUFFER_HEIGHT; y++) {
        for (x = 0; x < SCREEN_BUFFER_WIDTH; x++) {
            if (ctx.video[x + y * SCREEN_BUFFER_WIDTH]) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }

#ifndef NDEBUG
    draw_debug_info();
#endif
}

/* --- executor routines --------------------------------------------------- */

static void exec_raw(opcode_t *op)
{
    /* Intentionally left empty */
}

static void exec_cls(opcode_t *op)
{
    memset(ctx.video, 0x00, sizeof(ctx.video));
}

static void exec_ret(opcode_t *op)
{
    ctx.pc = stack_pop(&ctx.stack);    
}

static void exec_sys(opcode_t *op)
{
    /* Intentionally left empty */
}

static void exec_jp(opcode_t *op)
{
    if (op->addr_mode == AM_ADDR) {
        ctx.pc = op->address;
    } else if (op->addr_mode == AM_V0_ADDR) {
        ctx.pc = ctx.v[0x0] + op->address;
    }
}

static void exec_call(opcode_t *op)
{
    stack_push(&ctx.stack, ctx.pc);
    ctx.pc = op->address;
}

static void exec_se(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        if (ctx.v[op->x_reg] == op->byte) {
            ctx.pc += 2;
        }
    } else if (op->addr_mode == AM_VX_VY) {
        if (ctx.v[op->x_reg] == ctx.v[op->y_reg]) {
            ctx.pc += 2;
        }
    }
}

static void exec_sne(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        if (ctx.v[op->x_reg] != op->byte) {
            ctx.pc += 2;
        }
    } else if (op->addr_mode == AM_VX_VY) {
        if (ctx.v[op->x_reg] != ctx.v[op->y_reg]) {
            ctx.pc += 2;
        }
    }
}

static void exec_ld(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        ctx.v[op->x_reg] = op->byte;
    } else if (op->addr_mode == AM_VX_VY) {
        ctx.v[op->x_reg] = ctx.v[op->y_reg];
    } else if (op->addr_mode == AM_I_ADDR) {
        ctx.idx = op->address;
    } else if (op->addr_mode == AM_VX_DT) {
        ctx.v[op->x_reg] = ctx.dt;
    } else if (op->addr_mode == AM_VX_KEY) {
        uint8_t i, found;

        found = 0;
        for (i = 0; i < NUM_KEYS; i++) {
            if (ctx.keypad[i]) {
                found = 1;
                ctx.v[op->x_reg] = i;
                break;
            }
        }

        if (!found) {
            ctx.pc -= 2;
        }
    } else if (op->addr_mode == AM_DT_VX) {
        ctx.dt = ctx.v[op->x_reg];
    } else if (op->addr_mode == AM_ST_VX) {
        ctx.st = ctx.v[op->x_reg];
    } else if (op->addr_mode == AM_FONT_VX) {
        uint8_t digit;

        digit = ctx.v[op->x_reg];
        ctx.idx = ADDR_FONT + (5 * digit);
    } else if (op->addr_mode == AM_BCD_VX) {
	    uint8_t value;

        value = ctx.v[op->x_reg];
        ram_write(ctx.idx + 2, value % 10);
	    value /= 10;

        ram_write(ctx.idx + 1, value % 10);
	    value /= 10;

        ram_write(ctx.idx, value % 10);
    } else if (op->addr_mode == AM_ADDR_I_VX) {
        uint8_t i;
	    for (i = 0; i <= op->x_reg; ++i) {
            ram_write(ctx.idx + i, ctx.v[i]);
	    }
    } else if (op->addr_mode == AM_VX_ADDR_I) {
        uint8_t i;
        for (i = 0; i <= op->x_reg; i++) {
            ctx.v[i] = ram_read(ctx.idx + i);
        }
    }
}

static void exec_add(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        ctx.v[op->x_reg] += op->byte;
    } else if (op->addr_mode == AM_VX_VY) {
        uint16_t sum;

        sum = ctx.v[op->x_reg] + ctx.v[op->y_reg];
        if (sum > 0x00FF) {
            ctx.v[0xF] = 1;
        } else {
            ctx.v[0xF] = 0;
        }

        ctx.v[op->x_reg] = sum & 0x00FF;
    } else if (op->addr_mode == AM_I_VX) {
        ctx.idx += ctx.v[op->x_reg];
    }
}

static void exec_or(opcode_t *op)
{
    ctx.v[op->x_reg] |= ctx.v[op->y_reg];
}

static void exec_and(opcode_t *op)
{
    ctx.v[op->x_reg] &= ctx.v[op->y_reg];
}

static void exec_xor(opcode_t *op)
{
    ctx.v[op->x_reg] ^= ctx.v[op->y_reg];
}

static void exec_sub(opcode_t *op)
{
    if (ctx.v[op->x_reg] > ctx.v[op->y_reg]) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] -= ctx.v[op->y_reg];
}

static void exec_shr(opcode_t *op)
{
    ctx.v[0xF] = ctx.v[op->x_reg] & 0x01;
    ctx.v[op->x_reg] >>= 1;
}

static void exec_subn(opcode_t *op)
{
    if (ctx.v[op->y_reg] > ctx.v[op->x_reg]) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] = ctx.v[op->y_reg] - ctx.v[op->x_reg];
}

static void exec_shl(opcode_t *op)
{
    ctx.v[0xF] = (ctx.v[op->x_reg] & 0x80) >> 7;
    ctx.v[op->x_reg] <<= 1;
}

static void exec_rnd(opcode_t *op)
{
    ctx.v[op->x_reg] = GetRandomValue(0, 255) & op->byte;
}

static void exec_drw(opcode_t *op)
{
    uint8_t xp, yp, sprite, sprite_px, *screen_px, r, c, height;

    height = op->nibble;
    xp = ctx.v[op->x_reg] % SCREEN_BUFFER_WIDTH;
    yp = ctx.v[op->y_reg] % SCREEN_BUFFER_HEIGHT;

    ctx.v[0xF] = 0;
    for (r = 0; r < height; r++) {
        sprite = ram_read(ctx.idx + r);
        for (c = 0; c < 8; c++) {
            sprite_px = sprite & (0x80 >> c);
            screen_px = &ctx.video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (sprite_px) {
                if (*screen_px == 0xFF) {
                    ctx.v[0xF] = 1;
                }

                *screen_px ^= 0xFF;
            }
        }
    }
}

static void exec_skp(opcode_t *op)
{
    uint8_t key;

    key = ctx.v[op->x_reg];
    if (ctx.keypad[key]) {
        ctx.pc += 2;
    }
}

static void exec_sknp(opcode_t *op)
{
    uint8_t key;

    key = ctx.v[op->x_reg];
    if (!ctx.keypad[key]) {
        ctx.pc += 2;
    }
}

/* --- utility functions --------------------------------------------------- */

#ifndef NDEBUG
static void draw_debug_info(void)
{
    uint8_t r;
    int offset;

    /* registers */
    offset = 0;
    for (r = 0; r < NUM_REGISTERS; r++) {
        DrawText(TextFormat("V%X: %d", r, ctx.v[r]), 5, 5 + offset, 20, WHITE);
        offset += 20;
    }

    /* index and program counter */
    DrawText(TextFormat("IDX: 0x%X", ctx.idx), 5 * 20, 5, 20, WHITE);
    DrawText(TextFormat("PC: 0x%X", ctx.pc), 5 * 20, 25, 20, WHITE);

    /* timers */
    DrawText(TextFormat("DT: %d", ctx.dt), 15 * 20, 5, 20, WHITE);
    DrawText(TextFormat("ST: %d", ctx.st), 15 * 20, 25, 20, WHITE);
}
#endif
