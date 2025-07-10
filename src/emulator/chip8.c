#include "chip8.h"
#include "cpu.h"
#include "emulator/rom.h"
#include "ram.h"

#include "client/config.h"

#include "core/memory.h"
#include "core/types.h"

#include "renderer/renderer.h"

#include <raylib.h>

struct Chip8
{
    Chip8RAM  ram;
    Chip8CPU  cpu;
    Chip8Bool romLoaded;
};

static void c8ProcessInput(Chip8 *emulator);

Chip8 *c8InitEmulator(void)
{
    Chip8 *const emulator = C8_MALLOC(Chip8, 1);
    emulator->ram = c8InitRAM();
    emulator->cpu = c8InitCPU();
    emulator->romLoaded = C8_FALSE;

    return emulator;
}

void c8CloseEmulator(Chip8 *emulator)
{
    C8_FREE(emulator);
}

void c8LoadROMInEmulator(Chip8 *emulator, const char *romFile)
{
    const Chip8ROM rom = c8LoadROM(romFile);
    if (!rom.data)
    {
        emulator->romLoaded = C8_FALSE;
        return;
    }

    c8UploadROMToRAM(&emulator->ram, rom);
    SetWindowTitle(TextFormat("%s - %s", C8_WINDOW_TITLE, rom.romName));
    c8UnloadROM(rom);

    emulator->romLoaded = C8_TRUE;
}

void c8EmulatorOnUpdate(Chip8 *emulator)
{
    if (!emulator->romLoaded)
    {
        return;
    }

    c8ProcessInput(emulator);
    c8StepCPU(&emulator->cpu, &emulator->ram);
}

void c8EmulatorOnRender(const Chip8* emulator, Chip8Renderer *renderer)
{
    c8DrawBuffer(renderer, emulator->cpu.video, C8_SCREEN_BUFFER_WIDTH, C8_SCREEN_BUFFER_HEIGHT);

    if (c8DebugOverlayEnabled(renderer))
    {
        c8AddDebugText(renderer, "CPU:");
        c8AddDebugText(renderer, " - Registers:", (i32)emulator->cpu.v[0], (i32)emulator->cpu.v[1], (i32)emulator->cpu.v[2], (i32)emulator->cpu.v[3]);
        c8AddDebugText(renderer, "   (V0=%x V1=%x V2=%x V3=%x)", (i32)emulator->cpu.v[0], (i32)emulator->cpu.v[1], (i32)emulator->cpu.v[2], (i32)emulator->cpu.v[3]);
        c8AddDebugText(renderer, "   (V4=%x V5=%x V6=%x V7=%x)", (i32)emulator->cpu.v[4], (i32)emulator->cpu.v[5], (i32)emulator->cpu.v[6], (i32)emulator->cpu.v[7]);
        c8AddDebugText(renderer, "   (V8=%x V9=%x VA=%x VB=%x)", (i32)emulator->cpu.v[8], (i32)emulator->cpu.v[9], (i32)emulator->cpu.v[10], (i32)emulator->cpu.v[11]);
        c8AddDebugText(renderer, "   (VC=%x VD=%x VE=%x VF=%x)", (i32)emulator->cpu.v[12], (i32)emulator->cpu.v[13], (i32)emulator->cpu.v[14], (i32)emulator->cpu.v[15]);

        c8AddDebugText(renderer, " - Index register: %x", emulator->cpu.idx);
        c8AddDebugText(renderer, " - Program counter: %x", emulator->cpu.pc);

        c8AddDebugText(renderer, " - Delay timer: %x", (i32)emulator->cpu.dt);
        c8AddDebugText(renderer, " - Sound timer: %x", (i32)emulator->cpu.st);

        c8AddDebugText(renderer, "ROM:");
        c8AddDebugText(renderer, " - Loaded: %s", emulator->romLoaded ? "true" : "false");
    }
}

static void c8ProcessInput(Chip8 *emulator)
{
    const KeyboardKey keys[C8_NUM_KEYS] = {
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

    for (u8 k = 0; k < C8_NUM_KEYS; k++)
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
