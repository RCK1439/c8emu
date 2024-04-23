#include "cpu.h"
#include "constants.h"
#include "instructions.h"
#include "ram.h"
#include "util.h"

#include <raylib.h>
#include <stdint.h>
#include <assert.h>

typedef struct cpu_context_s {
    uint8_t v[NUM_REGISTERS];
    
    uint16_t i;
    uint16_t sp;
    uint16_t pc;

    uint8_t dt;
    uint8_t st;
} cpu_context_t;

typedef void (*exec_t)(opcode_t *op);

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

void cpu_init(void)
{
    ctx = (cpu_context_t) { 0 };
    ctx.sp = ADDR_STACK;
    ctx.pc = ADDR_PC;
}

void cpu_step(void)
{
    uint16_t opcode;
    opcode_t op;

    opcode = ram_read(ctx.pc);
    op = decode_opcode(opcode);

    instr_executors[op.instr](&op);
}

static void exec_raw(opcode_t *op)
{
    /* Intentionally left empty */
}

static void exec_cls(opcode_t *op)
{
    size_t px_addr;

    for (px_addr = ADDR_SCREEN; px_addr < ADDR_SCREEN + (SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT); px_addr++) {
        ram_write(px_addr, 0x00);
    }
}

static void exec_ret(opcode_t *op)
{
    /* we don't want to get stack underflow */
    assert(ctx.sp - 1 >= ADDR_STACK);

    ctx.pc = ram_read(ADDR_STACK + ctx.sp - 1);
    ctx.sp -= sizeof(uint16_t);
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
        ctx.pc = op->address + ctx.v[0x0];
    }
}

static void exec_call(opcode_t *op)
{
    ram_write(ADDR_STACK + ctx.sp, ctx.pc);
    ctx.sp += sizeof(uint16_t);
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
        ctx.i = op->address;
    } else if (op->addr_mode == AM_VX_DT) {
        ctx.v[op->x_reg] = ctx.dt;
    } else if (op->addr_mode == AM_VX_KEY) {
        uint8_t key;

        while ((key = util_kp()) == 0xFF);
        ctx.v[op->x_reg] = key;
    } else if (op->addr_mode == AM_DT_VX) {
        ctx.dt = ctx.v[op->x_reg];
    } else if (op->addr_mode == AM_ST_VX) {
        ctx.st = ctx.v[op->y_reg];
    } else if (op->addr_mode == AM_FONT_VX) {
        uint8_t digit;

        digit = ctx.v[op->x_reg];
        ctx.i = ADDR_FONT + (5 * digit);
    } else if (op->addr_mode == AM_BCD_VX) {
        uint8_t value;

        value = ctx.v[op->x_reg];
        ram_write_u8(ctx.i + 2, value % 10);

        value /= 10;
        ram_write_u8(ctx.i + 1, value % 10);

        value /= 10;
        ram_write_u8(ctx.i + 0, value % 10);
    } else if (op->addr_mode == AM_ADDR_I_VX) {
        uint8_t x;
        for (x = 0; x < op->x_reg; x++) {
            ram_write(ctx.i + x, ctx.v[x]);
        }
    } else if (op->addr_mode == AM_VX_ADDR_I) {
        uint8_t x;
        for (x = 0; x < op->x_reg; x++) {
            ctx.v[x] = ram_read(ctx.i + x);
        }
    }
}

static void exec_add(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        ctx.v[op->x_reg] += (op->byte);
    } else if (op->addr_mode == AM_VX_VY) {
        uint16_t result;

        result = (uint16_t)ctx.v[op->x_reg] + (uint16_t)ctx.v[op->y_reg];
        if (result > 0x00FF) {
            ctx.v[0xF] = 1;
        } else {
            ctx.v[0xF] = 0;
        }

        ctx.v[op->x_reg] = (uint8_t)(result & 0x00FF);
    } else if (op->addr_mode == AM_I_VX) {
        ctx.i += (uint16_t)ctx.v[op->x_reg];
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
    if (ctx.v[op->x_reg] & 0x01) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

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
    if (ctx.v[op->x_reg] & 0x80) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] <<= 1;
}

static void exec_rnd(opcode_t *op)
{
    ctx.v[op->x_reg] = (uint8_t)GetRandomValue(0, 255) & 0xFF;
}

static void exec_drw(opcode_t *op)
{
    uint8_t xp, yp, sprite, sprite_px;
    uint32_t *screen_px;
    size_t r, c;

    ctx.v[0xF] = 0;
    xp = ctx.v[op->x_reg] % SCREEN_BUFFER_WIDTH;
    yp = ctx.v[op->y_reg] % SCREEN_BUFFER_HEIGHT;

    for (r = 0; r < SCREEN_BUFFER_HEIGHT; r++) {
        sprite = ram_read_u8(ctx.i + (uint16_t)r);
        for (c = 0; c < 8; c++) {
            sprite_px = sprite & (0x80 >> c);
            if (!sprite_px) {
                continue;
            }

            screen_px = (uint32_t*)ram_ptr(ADDR_SCREEN + ((xp + c) + (yp + r) * SCREEN_BUFFER_WIDTH));
            if (*screen_px == 0xFFFFFFFF) {
                ctx.v[0xF] = 1;
            }
            *screen_px ^= 0xFFFFFFFF;
        }
    }
}

static void exec_skp(opcode_t *op)
{
    if (ctx.v[op->x_reg] == util_kp()) {
        ctx.pc += 2;
    }
}

static void exec_sknp(opcode_t *op)
{
    if (ctx.v[op->x_reg] != util_kp()) {
        ctx.pc += 2;
    }
}