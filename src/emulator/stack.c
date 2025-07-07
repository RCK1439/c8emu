#include "stack.h"

#include "core/error.h"

Chip8CallStack c8InitStack(void)
{
    const Chip8CallStack s = {
        .data = { 0 },
        .ptr = 0
    };

    return s;
}

void c8StackPush(Chip8CallStack *s, u16 val)
{
    C8_ASSERT(s->ptr < STACK_SIZE, "Cannot push; call stack is full");
    s->data[(s->ptr)++] = val;
}

u16 c8StackPop(Chip8CallStack *s)
{
    C8_ASSERT(s->ptr > 0, "Cannot pop; call stack is empty");
    return s->data[--(s->ptr)];
}
