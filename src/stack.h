/**
 * @file   stack.h
 * @brief  Provides an interface for the memory stack of Chip-8.
 * @author Ruan C. Keet
 * @date   2024-04-28
 */

#ifndef STACK_H
#define STACK_H

#include "constants.h"

#include <stdint.h>

/* --- type definitions ---------------------------------------------------- */

typedef struct Stack {
    uint16_t data[STACK_SIZE];
    uint8_t ptr;
} Stack;

/* --- stack interface ----------------------------------------------------- */

/**
 * Initializes the stack.
 * 
 * @return
 *      A handle to an empty stack.
 */
Stack InitStack(void);

/**
 * Pushes `val` to `s`.
 * 
 * @param[out] s
 *      The stack to push `val` to.
 * @param[in] val
 *      The value to push to `s`.
 */
void StackPush(Stack *const s, uint16_t val);

/**
 * Pops the top value from `s`.
 * 
 * @param[out] s
 *      The stack to pop from.
 * 
 * @return
 *      The popped value from the stack.
 */
uint16_t StackPop(Stack *const s);

#endif /* STACK_H */
