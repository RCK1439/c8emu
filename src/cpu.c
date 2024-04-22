#include "cpu.h"
#include "instructions.h"
#include "ram.h"

#include <raylib.h>
#include <stdint.h>

#define NUM_REGISTERS 16
#define STACK_SIZE 32

#define SCREEN_ADDRESS 0x0F00
#define STACK_POINTER_ADDRESS 0x0FA0
#define PROGRAM_COUNTER_ADDRESS 0x0200

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

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
    ctx.sp = STACK_POINTER_ADDRESS;
    ctx.pc = PROGRAM_COUNTER_ADDRESS;
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
    ctx.pc += 1;
}

static void exec_cls(opcode_t *op)
{
    size_t px_addr;

    for (px_addr = SCREEN_ADDRESS; px_addr < SCREEN_ADDRESS + (SCREEN_WIDTH * SCREEN_HEIGHT); px_addr++) {
        ram_write(px_addr, 0x00);
    }

    ctx.pc += 1;
}

static void exec_ret(opcode_t *op)
{
    ctx.pc = ram_read(STACK_POINTER_ADDRESS + ctx.sp - 1);
    ctx.sp--;
}

static void exec_sys(opcode_t *op)
{
    /* Intentionally left empty */
    ctx.pc += 1;
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
    ram_write(STACK_POINTER_ADDRESS + ctx.sp, ctx.pc);
    ctx.sp++;
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
        ctx.pc++;
    } else if (op->addr_mode == AM_VX_VY) {
        ctx.v[op->x_reg] = ctx.v[op->y_reg];
        ctx.pc++;
    } else if (op->addr_mode == AM_I_ADDR) {
        ctx.i = op->address;
        ctx.pc++;
    } else if (op->addr_mode == AM_VX_DT) {
        ctx.v[op->x_reg] = ctx.dt;
        ctx.pc++;
    } else if (op->addr_mode == AM_VX_KEY) {
        /* TODO */
    } else if (op->addr_mode == AM_DT_VX) {
        ctx.dt = ctx.v[op->x_reg];
        ctx.pc++;
    } else if (op->addr_mode == AM_ST_VX) {
        ctx.st = ctx.v[op->y_reg];
        ctx.pc++;
    } else if (op->addr_mode == AM_FONT_VX) {
        /* TODO */
    } else if (op->addr_mode == AM_BCD_VX) {
        /* TODO */
    } else if (op->addr_mode == AM_ADDR_I_VX) {
        uint8_t x;
        for (x = 0; x < op->x_reg; x++) {
            ram_write(ctx.i + x, ctx.v[x]);
        }
        ctx.pc++;
    } else if (op->addr_mode == AM_VX_ADDR_I) {
        uint8_t x;
        for (x = 0; x < op->x_reg; x++) {
            ctx.v[x] = ram_read(ctx.i + x);
        }
        ctx.pc++;
    }
}

static void exec_add(opcode_t *op)
{
    if (op->addr_mode == AM_VX_BYTE) {
        ctx.v[op->x_reg] += (op->byte);
        ctx.pc++;
    } else if (op->addr_mode == AM_VX_VY) {
        ctx.v[op->x_reg] += ctx.v[op->y_reg];
        ctx.pc++;
    } else if (op->addr_mode == AM_I_VX) {
        ctx.i += ctx.v[op->x_reg];
        ctx.pc++;
    }
}

static void exec_or(opcode_t *op)
{
    ctx.v[op->x_reg] |= ctx.v[op->y_reg];
    ctx.pc++;
}

static void exec_and(opcode_t *op)
{
    ctx.v[op->x_reg] &= ctx.v[op->y_reg];
    ctx.pc++;
}

static void exec_xor(opcode_t *op)
{
    ctx.v[op->x_reg] ^= ctx.v[op->y_reg];
    ctx.pc++;
}

static void exec_sub(opcode_t *op)
{
    ctx.v[op->x_reg] -= ctx.v[op->y_reg];
    ctx.pc++;
}

static void exec_shr(opcode_t *op)
{
    ctx.v[op->x_reg] >>= 1;
    ctx.pc++;
}

static void exec_subn(opcode_t *op)
{
    if (ctx.v[op->y_reg] > ctx.v[op->x_reg]) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] = ctx.v[op->y_reg] - ctx.v[op->x_reg];
    ctx.pc++;
}

static void exec_shl(opcode_t *op)
{
    if (ctx.v[op->x_reg] & 0x80) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] <<= 1;
    ctx.pc++;
}

static void exec_rnd(opcode_t *op)
{
    ctx.v[op->x_reg] = (uint8_t)GetRandomValue(0, 255) & 0xFF;
    ctx.pc++;
}

static void exec_drw(opcode_t *op)
{
    /* TODO */
}

static void exec_skp(opcode_t *op)
{
    /* TODO */
}

static void exec_sknp(opcode_t *op)
{
    /* TODO */
}