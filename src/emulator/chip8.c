#include "chip8.h"
#include "cpu.h"
#include "ram.h"

#include "core/memory.h"

#include <raylib.h>

#define NUM_KEYS 16

struct Chip8
{
    Chip8CPU cpu;
    Chip8RAM ram;
};

static void c8ProcessInput(Chip8 *emulator);

Chip8 *c8InitEmulator(void)
{
    Chip8 *const emulator = C8_MALLOC(Chip8, 1);
    emulator->cpu = c8InitCPU();
    emulator->ram = c8InitRAM();

    return emulator;
}

void c8CloseEmulator(Chip8 *emulator)
{
    C8_FREE(emulator);
}

Chip8Bool c8LoadROMInEmulator(Chip8 *emulator, const char *romFile)
{
    const Chip8ROM rom = c8LoadROM(romFile);
    c8UploadROMToRAM(&emulator->ram, rom);
    c8UnloadROM(rom);

    return C8_TRUE;
}

void c8EmulatorOnUpdate(Chip8 *emulator)
{
    c8ProcessInput(emulator);
    c8StepCPU(&emulator->cpu, &emulator->ram);
}

void c8EmulatorOnRender(const Chip8* emulator)
{
    c8DrawCPUBuffer(&emulator->cpu);
}

static void c8ProcessInput(Chip8 *emulator)
{
    const KeyboardKey keys[NUM_KEYS] = {
        KEY_ZERO,
        KEY_ONE,
        KEY_TWO,
        KEY_THREE,
        KEY_FOUR,
        KEY_FIVE,
        KEY_SIX,
        KEY_SEVEN,
        KEY_EIGHT,
        KEY_NINE,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F
    };

    for (u8 k = 0; k < NUM_KEYS; k++)
    {
        if (IsKeyDown(keys[k]))
        {
            c8SetCPUKey(&emulator->cpu, k, 1);
        }
        else
        {
            c8SetCPUKey(&emulator->cpu, k, 0);
        }
    }
}
