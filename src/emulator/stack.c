#include "stack.h"

#include "core/debug.h"

C8CallStack c8InitStack(void)
{
    const C8CallStack s = { 0 };
    return s;
}

void c8StackPush(C8CallStack *s, u16 val)
{
    C8_ASSERT(s->ptr < C8_CALLSTACK_SIZE, "Cannot push; call stack is full");
    s->data[(s->ptr)++] = val;
}

u16 c8StackPop(C8CallStack *s)
{
    C8_ASSERT(s->ptr > 0, "Cannot pop; call stack is empty");
    return s->data[--(s->ptr)];
}
