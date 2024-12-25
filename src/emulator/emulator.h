#ifndef EMULATOR_H
#define EMULATOR_H

typedef struct Emulator Emulator;

Emulator *CreateEmulator(void);
void DestroyEmulator(Emulator *emulator);

#endif /* EMULATOR_H */
