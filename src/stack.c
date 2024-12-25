#include "stack.h"

#include <assert.h>

CallStack CreateStack(void)
{
    const CallStack s = {
        .data = { 0 },
        .ptr = 0
    };

    return s;
}

void StackPush(CallStack *const s, uint16_t val)
{
    assert(s->ptr < STACK_SIZE);
    s->data[(s->ptr)++] = val;
}

uint16_t StackPop(CallStack *const s)
{
    assert(s->ptr > 0);
    return s->data[--(s->ptr)];
}
