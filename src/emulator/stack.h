#ifndef STACK_H
#define STACK_H

#include "specifications.h"

#include "core/types.h"

typedef struct C8CallStack
{
    u16 data[C8_CALLSTACK_SIZE];
    u8  ptr;
} C8CallStack;

C8CallStack c8InitStack(void);

void c8StackPush(C8CallStack *s, u16 val);
u16 c8StackPop(C8CallStack *s);

#endif /* STACK_H */

