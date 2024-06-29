/**
 * @file   stack.c
 * @brief  Implementation of the memory stack for Chip-8.
 * @author Ruan C. Keet
 * @file   2024-04-28
 */

#include "stack.h"

#include <assert.h>

/* --- stack interface ----------------------------------------------------- */

stack_t stack_init(void) {
    const stack_t s = {
        .data = { 0 },
        .ptr = 0
    };

    return s;
}

void stack_push(stack_t *const s, uint16_t val) {
    assert(s->ptr < STACK_SIZE);
    s->data[(s->ptr)++] = val;
}

uint16_t stack_pop(stack_t *const s) {
    assert(s->ptr > 0);
    return s->data[--(s->ptr)];
}

