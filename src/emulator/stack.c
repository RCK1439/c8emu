#include "stack.h"
#include "util.h"

CallStack CreateStack(void)
{
    const CallStack s = {
        .data = { 0 },
        .ptr = 0
    };

    return s;
}

void StackPush(CallStack *s, uint16_t val)
{
    C8_ASSERT(s->ptr < STACK_SIZE, "Cannot push; call stack is full");
    s->data[(s->ptr)++] = val;
}

uint16_t StackPop(CallStack *s)
{
    C8_ASSERT(s->ptr > 0, "Cannot pop; call stack is empty");
    return s->data[--(s->ptr)];
}
