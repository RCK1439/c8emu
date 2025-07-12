#include "chip8.h"
#include "cpu.h"
#include "keyboard.h"
#include "ram.h"
#include "rom.h"

#include "client/config.h"

#include "core/memory.h"
#include "core/types.h"

#include "renderer/renderer.h"

#include <raylib.h>

// --- struct definitions -----------------------------------------------------

// Definition of `Chip8` handle
struct Chip8
{
    Chip8RAM  ram;       // The Chip-8 memory
    Chip8CPU  cpu;       // The Chip-8 processor
    Chip8Bool romLoaded; // Flag indicating whether a ROM is loaded or not
};

// --- static functions -------------------------------------------------------

/**
 * Processes any input by the user on their host-machine and sends the relevant
 * input data to the emulator
 *
 * @param[in] emulator
 *      The handle to the emulator
 */
static void c8ProcessInput(Chip8 *emulator);

// --- chip-8 interface -------------------------------------------------------

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
    c8ProcessInput(emulator);
    if (emulator->romLoaded)
    {
        c8StepCPU(&emulator->cpu, &emulator->ram);
    }
}

void c8EmulatorOnRender(const Chip8* emulator, Chip8Renderer *renderer)
{
    c8DrawBuffer(renderer, emulator->cpu.video, C8_SCREEN_BUFFER_WIDTH, C8_SCREEN_BUFFER_HEIGHT);

    if (c8DebugOverlayEnabled(renderer))
    {
        c8AddDebugText(renderer, "CPU:");
        c8AddDebugText(renderer, "- Registers:");
        c8AddDebugText(renderer, "  (V0=%x V1=%x V2=%x V3=%x)",
            (i32)emulator->cpu.registers[V0],
            (i32)emulator->cpu.registers[V1],
            (i32)emulator->cpu.registers[V2],
            (i32)emulator->cpu.registers[V3]
        );
        c8AddDebugText(renderer, "  (V4=%x V5=%x V6=%x V7=%x)",
            (i32)emulator->cpu.registers[V4],
            (i32)emulator->cpu.registers[V5],
            (i32)emulator->cpu.registers[V6],
            (i32)emulator->cpu.registers[V7]
        );
        c8AddDebugText(renderer, "  (V8=%x V9=%x VA=%x VB=%x)",
            (i32)emulator->cpu.registers[V8],
            (i32)emulator->cpu.registers[V9],
            (i32)emulator->cpu.registers[VA],
            (i32)emulator->cpu.registers[VB]
        );
        c8AddDebugText(renderer, "  (VC=%x VD=%x VE=%x VF=%x)",
            (i32)emulator->cpu.registers[VC],
            (i32)emulator->cpu.registers[VD],
            (i32)emulator->cpu.registers[VE],
            (i32)emulator->cpu.registers[VF]
        );

        c8AddDebugText(renderer, "- Index register: %x", emulator->cpu.idx);
        c8AddDebugText(renderer, "- Program counter: %x", emulator->cpu.pc);

        c8AddDebugText(renderer, "- Delay timer: %x", (i32)emulator->cpu.dt);
        c8AddDebugText(renderer, "- Sound timer: %x", (i32)emulator->cpu.st);

        c8AddDebugText(renderer, "- Keypad:");
        c8AddDebugText(renderer, "  [K1=%d, K2=%d, K3=%d, KC=%d]",
            (i32)emulator->cpu.keypad[0x1],
            (i32)emulator->cpu.keypad[0x2],
            (i32)emulator->cpu.keypad[0x3],
            (i32)emulator->cpu.keypad[0xC]
        );
        c8AddDebugText(renderer, "  [K4=%d, K5=%d, K6=%d, KD=%d]",
            (i32)emulator->cpu.keypad[0x4],
            (i32)emulator->cpu.keypad[0x5],
            (i32)emulator->cpu.keypad[0x6],
            (i32)emulator->cpu.keypad[0xD]
        );
        c8AddDebugText(renderer, "  [K7=%d, K8=%d, K9=%d, KE=%d]",
            (i32)emulator->cpu.keypad[0x7],
            (i32)emulator->cpu.keypad[0x8],
            (i32)emulator->cpu.keypad[0x9],
            (i32)emulator->cpu.keypad[0xE]
        );
        c8AddDebugText(renderer, "  [KA=%d, K0=%d, KB=%d, KF=%d]",
            (i32)emulator->cpu.keypad[0xA],
            (i32)emulator->cpu.keypad[0x0],
            (i32)emulator->cpu.keypad[0xB],
            (i32)emulator->cpu.keypad[0xF]
        );
    }
}

// --- static function implementations ----------------------------------------

static void c8ProcessInput(Chip8 *emulator)
{
    const Chip8Key keys[C8_NUM_KEYS] = {
        C8_KEY_0,
        C8_KEY_1,
        C8_KEY_2,
        C8_KEY_3,
        C8_KEY_4,
        C8_KEY_5,
        C8_KEY_6,
        C8_KEY_7,
        C8_KEY_8,
        C8_KEY_9,
        C8_KEY_A,
        C8_KEY_B,
        C8_KEY_C,
        C8_KEY_D,
        C8_KEY_E,
        C8_KEY_F,
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
