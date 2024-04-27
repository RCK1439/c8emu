#include "stack.h"

#include <assert.h>

stack_t stack_init(void)
{
    return (stack_t){ 0 };
}

void stack_push(stack_t *s, uint16_t val)
{
    assert(s->ptr < STACK_SIZE);
    s->data[(s->ptr)++] = val;
}

uint16_t stack_pop(stack_t *s)
{
    assert(s->ptr > 0);
    return s->data[--(s->ptr)];
}
