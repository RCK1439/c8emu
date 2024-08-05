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
typedef struct cpu_s {
    uint8_t v[NUM_REGISTERS];
    
    uint16_t idx;
    uint16_t pc;

    uint8_t dt;
    uint8_t st;

    stack_t stack;
    uint8_t video[SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT];
    uint8_t keypad[NUM_KEYS];
} cpu_t;

/**
 * Defines a function signature for an executor routine.
 */
typedef void (*exec_t)(const opcode_t *const op);

/* --- executor routines --------------------------------------------------- */

static void raw(const opcode_t *const op);
static void cls(const opcode_t *const op);
static void ret(const opcode_t *const op);
static void sys(const opcode_t *const op);
static void jp(const opcode_t *const op);
static void call(const opcode_t *const op);
static void se(const opcode_t *const op);
static void sne(const opcode_t *const op);
static void ld(const opcode_t *const op);
static void add(const opcode_t *const op);
static void or(const opcode_t *const op);
static void and(const opcode_t *const op);
static void xor(const opcode_t *const op);
static void sub(const opcode_t *const op);
static void shr(const opcode_t *const op);
static void subn(const opcode_t *const op);
static void shl(const opcode_t *const op);
static void rnd(const opcode_t *const op);
static void drw(const opcode_t *const op);
static void skp(const opcode_t *const op);
static void sknp(const opcode_t *const op);

/* --- global variables ---------------------------------------------------- */

static cpu_t cpu;
static exec_t executors[] = {
    [IN_RAW] = raw,
    [IN_CLS] = cls,
    [IN_RET] = ret,
    [IN_SYS] = sys,
    [IN_JP] = jp,
    [IN_CALL] = call,
    [IN_SE] = se,
    [IN_SNE] = sne,
    [IN_LD] = ld,
    [IN_ADD] = add,
    [IN_OR] = or,
    [IN_AND] = and,
    [IN_XOR] = xor,
    [IN_SUB] = sub,
    [IN_SHR] = shr,
    [IN_SUBN] = subn,
    [IN_SHL] = shl,
    [IN_RND] = rnd,
    [IN_DRW] = drw,
    [IN_SKP] = skp,
    [IN_SKNP] = sknp
};

/* --- cpu interface ------------------------------------------------------- */

void cpu_init(void) {
    memset(cpu.v, 0x00, sizeof(cpu.v));
    cpu.idx = 0;
    cpu.pc = ADDR_PC;
    cpu.dt = 0;
    cpu.st = 0;
    cpu.stack = stack_init();
    memset(cpu.video, 0x00, sizeof(cpu.video));
    memset(cpu.keypad, 0x00, sizeof(cpu.keypad));
}

void cpu_step(void) {
    const uint16_t raw_op = (ram_read(cpu.pc) << 8) | ram_read(cpu.pc + 1);
    cpu.pc += 2;

    const opcode_t opcode = decode_opcode(raw_op);
    executors[opcode.instr](&opcode);

    if (cpu.dt > 0) {
        cpu.dt--;
    }

    if (cpu.st > 0) {
        cpu.st--;
    }
}

void cpu_set_key(uint8_t key, uint8_t val) {
    cpu.keypad[key] = val;
}

void cpu_draw_buffer(void) {
    for (uint16_t y = 0; y < SCREEN_BUFFER_HEIGHT; y++) {
        for (uint16_t x = 0; x < SCREEN_BUFFER_WIDTH; x++) {
            if (cpu.video[x + y * SCREEN_BUFFER_WIDTH]) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }

    DRAW_DEBUG_INFO(cpu.v, cpu.dt, cpu.st, cpu.idx, cpu.pc, cpu.keypad);
}

/* --- executor routines --------------------------------------------------- */

static void raw(unused const opcode_t *const op) {
    /* Intentionally left empty */
}

static void cls(unused const opcode_t *const op) {
    memset(cpu.video, 0x00, sizeof(cpu.video));
}

static void ret(unused const opcode_t *const op) {
    cpu.pc = stack_pop(&cpu.stack);    
}

static void sys(unused const opcode_t *const op) {
    /* Intentionally left empty */
}

static void jp(const opcode_t *const op) {
    if (op->addr_mode == AM_ADDR) {
        cpu.pc = op->address;
    } else if (op->addr_mode == AM_V0_ADDR) {
        cpu.pc = cpu.v[0x0] + op->address;
    }
}

static void call(const opcode_t *const op) {
    stack_push(&cpu.stack, cpu.pc);
    cpu.pc = op->address;
}

static void se(const opcode_t *const op) {
    if (op->addr_mode == AM_VX_BYTE) {
        if (cpu.v[op->x_reg] == op->byte) {
            cpu.pc += 2;
        }
    } else if (op->addr_mode == AM_VX_VY) {
        if (cpu.v[op->x_reg] == cpu.v[op->y_reg]) {
            cpu.pc += 2;
        }
    }
}

static void sne(const opcode_t *const op) {
    if (op->addr_mode == AM_VX_BYTE) {
        if (cpu.v[op->x_reg] != op->byte) {
            cpu.pc += 2;
        }
    } else if (op->addr_mode == AM_VX_VY) {
        if (cpu.v[op->x_reg] != cpu.v[op->y_reg]) {
            cpu.pc += 2;
        }
    }
}

static void ld(const opcode_t *const op) {
    if (op->addr_mode == AM_VX_BYTE) {
        cpu.v[op->x_reg] = op->byte;
    } else if (op->addr_mode == AM_VX_VY) {
        cpu.v[op->x_reg] = cpu.v[op->y_reg];
    } else if (op->addr_mode == AM_I_ADDR) {
        cpu.idx = op->address;
    } else if (op->addr_mode == AM_VX_DT) {
        cpu.v[op->x_reg] = cpu.dt;
    } else if (op->addr_mode == AM_VX_KEY) {
        bool found = false;
        for (uint8_t i = 0; i < NUM_KEYS; i++) {
            if (cpu.keypad[i]) {
                found = true;
                cpu.v[op->x_reg] = i;
                break;
            }
        }

        if (!found) {
            cpu.pc -= 2;
        }
    } else if (op->addr_mode == AM_DT_VX) {
        cpu.dt = cpu.v[op->x_reg];
    } else if (op->addr_mode == AM_ST_VX) {
        cpu.st = cpu.v[op->x_reg];
    } else if (op->addr_mode == AM_FONT_VX) {
        const uint8_t digit = cpu.v[op->x_reg];
        cpu.idx = ADDR_FONT + (5 * digit);
    } else if (op->addr_mode == AM_BCD_VX) {
        uint8_t value = cpu.v[op->x_reg];
        ram_write(cpu.idx + 2, value % 10);
	    value /= 10;

        ram_write(cpu.idx + 1, value % 10);
	    value /= 10;

        ram_write(cpu.idx, value % 10);
    } else if (op->addr_mode == AM_ADDR_I_VX) {
	    for (uint8_t i = 0; i <= op->x_reg; i++) {
            ram_write(cpu.idx + i, cpu.v[i]);
        }
    } else if (op->addr_mode == AM_VX_ADDR_I) {
        for (uint8_t i = 0; i <= op->x_reg; i++) {
            cpu.v[i] = ram_read(cpu.idx + i);
        }
    }
}

static void add(const opcode_t *const op) {
    if (op->addr_mode == AM_VX_BYTE) {
        cpu.v[op->x_reg] += op->byte;
    } else if (op->addr_mode == AM_VX_VY) {
        const uint16_t sum = cpu.v[op->x_reg] + cpu.v[op->y_reg];
        if (sum > 0x00FF) {
            cpu.v[0xF] = 1;
        } else {
            cpu.v[0xF] = 0;
        }

        cpu.v[op->x_reg] = sum & 0x00FF;
    } else if (op->addr_mode == AM_I_VX) {
        cpu.idx += cpu.v[op->x_reg];
    }
}

static void or(const opcode_t *const op) {
    cpu.v[op->x_reg] |= cpu.v[op->y_reg];
}

static void and(const opcode_t *const op) {
    cpu.v[op->x_reg] &= cpu.v[op->y_reg];
}

static void xor(const opcode_t *const op) {
    cpu.v[op->x_reg] ^= cpu.v[op->y_reg];
}

static void sub(const opcode_t *const op) {
    if (cpu.v[op->x_reg] > cpu.v[op->y_reg]) {
        cpu.v[0xF] = 1;
    } else {
        cpu.v[0xF] = 0;
    }

    cpu.v[op->x_reg] -= cpu.v[op->y_reg];
}

static void shr(const opcode_t *const op) {
    cpu.v[0xF] = cpu.v[op->x_reg] & 0x01;
    cpu.v[op->x_reg] >>= 1;
}

static void subn(const opcode_t *const op) {
    if (cpu.v[op->y_reg] > cpu.v[op->x_reg]) {
        cpu.v[0xF] = 1;
    } else {
        cpu.v[0xF] = 0;
    }

    cpu.v[op->x_reg] = cpu.v[op->y_reg] - cpu.v[op->x_reg];
}

static void shl(const opcode_t *const op) {
    cpu.v[0xF] = (cpu.v[op->x_reg] & 0x80) >> 7;
    cpu.v[op->x_reg] <<= 1;
}

static void rnd(const opcode_t *const op) {
    cpu.v[op->x_reg] = GetRandomValue(0, 255) & op->byte;
}

static void drw(const opcode_t *const op) {
    const uint8_t height = op->nibble;
    const uint8_t xp = cpu.v[op->x_reg] % SCREEN_BUFFER_WIDTH;
    const uint8_t yp = cpu.v[op->y_reg] % SCREEN_BUFFER_HEIGHT;

    cpu.v[0xF] = 0;
    for (uint8_t r = 0; r < height; r++) {
        const uint8_t sprite = ram_read(cpu.idx + r);
        for (uint8_t c = 0; c < 8; c++) {
            const uint8_t sprite_px = sprite & (0x80 >> c);
            uint8_t *const screen_px = &cpu.video[(yp + r) * SCREEN_BUFFER_WIDTH + (xp + c)];

            if (sprite_px) {
                if (*screen_px == 0xFF) {
                    cpu.v[0xF] = 1;
                }

                *screen_px ^= 0xFF;
            }
        }
    }
}

static void skp(const opcode_t *const op) {
    const uint8_t key = cpu.v[op->x_reg];
    if (cpu.keypad[key]) {
        cpu.pc += 2;
    }
}

static void sknp(const opcode_t *const op) {
    const uint8_t key = cpu.v[op->x_reg];
    if (!cpu.keypad[key]) {
        cpu.pc += 2;
    }
}

