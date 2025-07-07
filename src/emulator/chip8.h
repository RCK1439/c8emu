#ifndef CHIP8_H
#define CHIP8_H

#include "core/types.h"

typedef struct Chip8 Chip8;

Chip8 *c8InitEmulator(void);
void c8CloseEmulator(Chip8 *emulator);

Chip8Bool c8LoadROMInEmulator(Chip8 *emulator, const char *romFile);

void c8EmulatorOnUpdate(Chip8 *emulator);
void c8EmulatorOnRender(const Chip8* emulator);

#endif /* CHIP8_H */
