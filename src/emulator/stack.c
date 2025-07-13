#include "stack.h"

#include "core/debug.h"

C8CallStack c8InitStack(void)
{
    const C8CallStack s = { 0 };
    return s;
}

void c8PushAddr(C8CallStack *s, u16 addr)
{
    C8_ASSERT(s->ptr < C8_CALLSTACK_SIZE, "Cannot push; call stack is full");
    s->data[(s->ptr)++] = addr;
}

u16 c8PopAddr(C8CallStack *s)
{
    C8_ASSERT(s->ptr > 0, "Cannot pop; call stack is empty");
    return s->data[--(s->ptr)];
}
