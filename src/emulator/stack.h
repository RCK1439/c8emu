#ifndef STACK_H
#define STACK_H

#include <stdint.h>

#define STACK_SIZE 32

typedef struct CallStack
{
    uint16_t data[STACK_SIZE];
    uint8_t  ptr;
} CallStack;

CallStack CreateStack(void);

void StackPush(CallStack *s, uint16_t val);
uint16_t StackPop(CallStack *s);

#endif /* STACK_H */

