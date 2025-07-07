#ifndef STACK_H
#define STACK_H

#include "specifications.h"

#include "core/types.h"

typedef struct Chip8CallStack
{
    u16 data[C8_CALLSTACK_SIZE];
    u8  ptr;
} Chip8CallStack;

Chip8CallStack c8InitStack(void);

void c8StackPush(Chip8CallStack *s, u16 val);
u16 c8StackPop(Chip8CallStack *s);

#endif /* STACK_H */

