#include "stack.h"

#include <assert.h>

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
