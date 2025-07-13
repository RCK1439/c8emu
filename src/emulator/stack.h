#ifndef STACK_H
#define STACK_H

#include "specifications.h"

#include "core/types.h"

// --- type definitions -------------------------------------------------------

/**
 * Struct representing the Chip-8 call stack
 */
typedef struct C8CallStack
{
    u16 data[C8_CALLSTACK_SIZE]; // The call stack buffer
    u8  ptr;                     // The current pointer on the stack
} C8CallStack;

// --- stack interface --------------------------------------------------------

/**
 * Initializes the Chip-8 call stack
 *
 * @return
 *      The initialized call stack
 */
C8CallStack c8InitStack(void);

/**
 * Pushes the function address onto the call stack
 *
 * @param[in] s
 *      A pointer to the call stack
 * @param[in] addr
 *      The function address to push onto the stack
 */
void c8PushAddr(C8CallStack *s, u16 addr);

/**
 * Pops the last function address from the call stack
 *
 * @param[in] s
 *      A pointer to the call stack
 *
 * @return
 *      The last called function address 
 */
u16 c8PopAddr(C8CallStack *s);

#endif /* STACK_H */

