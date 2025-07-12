#ifndef CPU_H
#define CPU_H

#include "keyboard.h"
#include "ram.h"
#include "specifications.h"
#include "stack.h"

#include "core/types.h"

typedef enum Chip8Register
{
    V0 = 0x00,
    V1 = 0x01,
    V2 = 0x02,
    V3 = 0x03,
    V4 = 0x04,
    V5 = 0x05,
    V6 = 0x06,
    V7 = 0x07,
    V8 = 0x08,
    V9 = 0x09,
    VA = 0x0A,
    VB = 0x0B,
    VC = 0x0C,
    VD = 0x0D,
    VE = 0x0E,
    VF = 0x0F
} Chip8Register;

typedef struct Chip8CPU
{
    u8             video[C8_SCREEN_BUFFER_WIDTH * C8_SCREEN_BUFFER_HEIGHT];
    Chip8CallStack stack;
    
    u8             keypad[C8_NUM_KEYS];
    u8             registers[C8_NUM_REGISTERS];
    
    u16            idx;
    u16            pc;

    u8             dt;
    u8             st;
} Chip8CPU;

Chip8CPU c8InitCPU(void);

void c8StepCPU(Chip8CPU *cpu, Chip8RAM *ram);
void c8SetCPUKey(Chip8CPU *cpu, Chip8Key key, u8 val);

#endif /* CPU_H */

