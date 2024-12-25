#ifndef STACK_H
#define STACK_H

#include "constants.h"

#include <stdint.h>

typedef struct stack_s {
    uint16_t data[STACK_SIZE];
    uint8_t ptr;
} stack_t;

stack_t stack_init(void);

void stack_push(stack_t *const s, uint16_t val);
uint16_t stack_pop(stack_t *const s);

#endif /* STACK_H */

