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
typedef void (*exec_t)(const opcode_t *const op);

/* --- executor routines --------------------------------------------------- */

static void exec_raw(const opcode_t *const op);
static void exec_cls(const opcode_t *const op);
static void exec_ret(const opcode_t *const op);
static void exec_sys(const opcode_t *const op);
static void exec_jp(const opcode_t *const op);
static void exec_call(const opcode_t *const op);
static void exec_se(const opcode_t *const op);
static void exec_sne(const opcode_t *const op);
static void exec_ld(const opcode_t *const op);
static void exec_add(const opcode_t *const op);
static void exec_or(const opcode_t *const op);
static void exec_and(const opcode_t *const op);
static void exec_xor(const opcode_t *const op);
static void exec_sub(const opcode_t *const op);
static void exec_shr(const opcode_t *const op);
static void exec_subn(const opcode_t *const op);
static void exec_shl(const opcode_t *const op);
static void exec_rnd(const opcode_t *const op);
static void exec_drw(const opcode_t *const op);
static void exec_skp(const opcode_t *const op);
static void exec_sknp(const opcode_t *const op);

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

void cpu_init(void) {
    memset(ctx.v, 0x00, sizeof(ctx.v));
    ctx.idx = 0;
    ctx.pc = ADDR_PC;
    ctx.dt = 0;
    ctx.st = 0;
    ctx.stack = stack_init();
    memset(ctx.video, 0x00, sizeof(ctx.video));
    memset(ctx.keypad, 0x00, sizeof(ctx.keypad));
}

void cpu_step(void) {
    const uint16_t raw_op = (ram_read(ctx.pc) << 8) | ram_read(ctx.pc + 1);
    ctx.pc += 2;

    const opcode_t opcode = decode_opcode(raw_op);
    instr_executors[opcode.instr](&opcode);

    if (ctx.dt > 0) {
        ctx.dt--;
    }

    if (ctx.st > 0) {
        ctx.st--;
    }
}

void cpu_setkey(uint8_t key, uint8_t val) {
    ctx.keypad[key] = val;
}

void cpu_draw_buffer(void) {
    for (uint16_t y = 0; y < SCREEN_BUFFER_HEIGHT; y++) {
        for (uint16_t x = 0; x < SCREEN_BUFFER_WIDTH; x++) {
            if (ctx.video[x + y * SCREEN_BUFFER_WIDTH]) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }

    DRAW_DEBUG_INFO(ctx.v, ctx.dt, ctx.st, ctx.idx, ctx.pc, ctx.keypad);
}

/* --- executor routines --------------------------------------------------- */

static void exec_raw(unused const opcode_t *const op) {
    /* Intentionally left empty */
}

static void exec_cls(unused const opcode_t *const op) {
    memset(ctx.video, 0x00, sizeof(ctx.video));
}

static void exec_ret(unused const opcode_t *const op) {
    ctx.pc = stack_pop(&ctx.stack);    
}

static void exec_sys(unused const opcode_t *const op) {
    /* Intentionally left empty */
}

static void exec_jp(const opcode_t *const op) {
    if (op->addr_mode == AM_ADDR) {
        ctx.pc = op->address;
    } else if (op->addr_mode == AM_V0_ADDR) {
        ctx.pc = ctx.v[0x0] + op->address;
    }
}

static void exec_call(const opcode_t *const op) {
    stack_push(&ctx.stack, ctx.pc);
    ctx.pc = op->address;
}

static void exec_se(const opcode_t *const op) {
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

static void exec_sne(const opcode_t *const op) {
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

static void exec_ld(const opcode_t *const op) {
    if (op->addr_mode == AM_VX_BYTE) {
        ctx.v[op->x_reg] = op->byte;
    } else if (op->addr_mode == AM_VX_VY) {
        ctx.v[op->x_reg] = ctx.v[op->y_reg];
    } else if (op->addr_mode == AM_I_ADDR) {
        ctx.idx = op->address;
    } else if (op->addr_mode == AM_VX_DT) {
        ctx.v[op->x_reg] = ctx.dt;
    } else if (op->addr_mode == AM_VX_KEY) {
        
        bool found = false;
        for (uint8_t i = 0; i < NUM_KEYS; i++) {
            if (ctx.keypad[i]) {
                found = true;
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

static void exec_add(const opcode_t *const op) {
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

static void exec_or(const opcode_t *const op) {
    ctx.v[op->x_reg] |= ctx.v[op->y_reg];
}

static void exec_and(const opcode_t *const op) {
    ctx.v[op->x_reg] &= ctx.v[op->y_reg];
}

static void exec_xor(const opcode_t *const op) {
    ctx.v[op->x_reg] ^= ctx.v[op->y_reg];
}

static void exec_sub(const opcode_t *const op) {
    if (ctx.v[op->x_reg] > ctx.v[op->y_reg]) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] -= ctx.v[op->y_reg];
}

static void exec_shr(const opcode_t *const op) {
    ctx.v[0xF] = ctx.v[op->x_reg] & 0x01;
    ctx.v[op->x_reg] >>= 1;
}

static void exec_subn(const opcode_t *const op) {
    if (ctx.v[op->y_reg] > ctx.v[op->x_reg]) {
        ctx.v[0xF] = 1;
    } else {
        ctx.v[0xF] = 0;
    }

    ctx.v[op->x_reg] = ctx.v[op->y_reg] - ctx.v[op->x_reg];
}

static void exec_shl(const opcode_t *const op) {
    ctx.v[0xF] = (ctx.v[op->x_reg] & 0x80) >> 7;
    ctx.v[op->x_reg] <<= 1;
}

static void exec_rnd(const opcode_t *const op) {
    ctx.v[op->x_reg] = GetRandomValue(0, 255) & op->byte;
}

static void exec_drw(const opcode_t *const op) {
    const uint8_t height = op->nibble;
    const uint8_t xp = ctx.v[op->x_reg] % SCREEN_BUFFER_WIDTH;
    const uint8_t yp = ctx.v[op->y_reg] % SCREEN_BUFFER_HEIGHT;

    ctx.v[0xF] = 0;
    for (uint8_t r = 0; r < height; r++) {
        const uint8_t sprite = ram_read(ctx.idx + r);
        for (uint8_t c = 0; c < 8; c++) {
            const uint8_t sprite_px = sprite & (0x80 >> c);
            uint8_t *const screen_px = &ctx.video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (sprite_px) {
                if (*screen_px == 0xFF) {
                    ctx.v[0xF] = 1;
                }

                *screen_px ^= 0xFF;
            }
        }
    }
}

static void exec_skp(const opcode_t *const op) {
    const uint8_t key = ctx.v[op->x_reg];
    if (ctx.keypad[key]) {
        ctx.pc += 2;
    }
}

static void exec_sknp(const opcode_t *const op) {
    const uint8_t key = ctx.v[op->x_reg];
    if (!ctx.keypad[key]) {
        ctx.pc += 2;
    }
}

