#ifndef STACK_H
#define STACK_H

#include "core/types.h"

#define STACK_SIZE 32

typedef struct Chip8CallStack
{
    u16 data[STACK_SIZE];
    u8  ptr;
} Chip8CallStack;

Chip8CallStack c8InitStack(void);

void c8StackPush(Chip8CallStack *s, u16 val);
u16 c8StackPop(Chip8CallStack *s);

#endif /* STACK_H */

