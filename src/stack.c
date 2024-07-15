/**
 * @file   stack.c
 * @brief  Implementation of the memory stack for Chip-8.
 * @author Ruan C. Keet
 * @file   2024-04-28
 */

#include "stack.h"

#include <assert.h>

/* --- stack interface ----------------------------------------------------- */

Stack InitStack(void)
{
    const Stack s = {
        .data = { 0 },
        .ptr = 0
    };

    return s;
}

void StackPush(Stack *const s, uint16_t val)
{
    assert(s->ptr < STACK_SIZE);
    s->data[(s->ptr)++] = val;
}

uint16_t StackPop(Stack *const s)
{
    assert(s->ptr > 0);
    return s->data[--(s->ptr)];
}

