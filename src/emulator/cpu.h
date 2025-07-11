#ifndef CPU_H
#define CPU_H

#include "keyboard.h"
#include "ram.h"
#include "specifications.h"
#include "stack.h"

#include "core/types.h"

typedef struct Chip8CPU
{
    u8             video[C8_SCREEN_BUFFER_WIDTH * C8_SCREEN_BUFFER_HEIGHT];
    Chip8CallStack stack;
    
    u8             keypad[C8_NUM_KEYS];
    u8             v[C8_NUM_REGISTERS];
    
    u16            idx;
    u16            pc;

    u8             dt;
    u8             st;
} Chip8CPU;

Chip8CPU c8InitCPU(void);

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram);
void c8SetCPUKey(Chip8CPU *cpu, Chip8Key key, u8 val);

#endif /* CPU_H */

