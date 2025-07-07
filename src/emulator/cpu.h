#ifndef CPU_H
#define CPU_H

#include "ram.h"
#include "stack.h"

#include "core/types.h"

#define NUM_REGISTERS 16
#define NUM_KEYS 16

#define SCREEN_BUFFER_WIDTH 64
#define SCREEN_BUFFER_HEIGHT 32
#define SCALE 16

typedef struct Chip8CPU
{
    u8             video[SCREEN_BUFFER_WIDTH * SCREEN_BUFFER_HEIGHT];
    Chip8CallStack stack;
    
    u8             keypad[NUM_KEYS];
    u8             v[NUM_REGISTERS];
    
    u16            idx;
    u16            pc;

    u8             dt;
    u8             st;
} Chip8CPU;

Chip8CPU c8InitCPU(void);

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram);
void c8SetCPUKey(Chip8CPU *cpu, u8 key, u8 val);

void c8DrawCPUBuffer(const Chip8CPU *cpu);

#endif /* CPU_H */

