#ifndef CPU_H
#define CPU_H

#include "ram.h"

#include "core/types.h"

typedef struct Chip8CPU Chip8CPU;

Chip8CPU *c8InitCPU(void);
void c8CloseCPU(Chip8CPU *cpu);

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram);
void c8SetCPUKey(Chip8CPU *cpu, u8 key, u8 val);

void c8DrawCPUBuffer(const Chip8CPU *cpu);

#endif /* CPU_H */

