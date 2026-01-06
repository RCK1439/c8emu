#include "Chip8.hpp"
#include "CPU.hpp"
#include "Keyboard.hpp"
#include "RAM.hpp"
#include "ROM.hpp"

#include "Core/Types.hpp"

#include "Renderer/Renderer.hpp"

#include <raylib.h>

namespace c8emu {

void Chip8::LoadROM(const ROM& rom) noexcept
{
    m_RAM.LoadROM(rom);
    m_ROMLoaded = true;
}

void Chip8::OnUpdate() noexcept
{
    ProcessInput();
    if (m_Tick >= C8_TICK_RATE)
    {
        if (m_ROMLoaded)
        {
            m_CPU.Step(m_RAM);
        }

        m_Tick -= C8_TICK_RATE;
    }

    m_Tick += ::GetFrameTime();
}

void Chip8::OnRender(Renderer& renderer) const noexcept
{
    renderer.DrawBuffer(m_CPU.m_Video.data(), C8_SCREEN_BUFFER_WIDTH, C8_SCREEN_BUFFER_HEIGHT);

    if (renderer.DebugOverlayEnabled())
    {
        renderer.AddDebugText("CPU:");
        renderer.AddDebugText("- Registers:");
        renderer.AddDebugText("  (V0={} V1={} V2={} V3={})",
            m_CPU.m_Registers[RegisterID::V0],
            m_CPU.m_Registers[RegisterID::V1],
            m_CPU.m_Registers[RegisterID::V2],
            m_CPU.m_Registers[RegisterID::V3]
        );
        renderer.AddDebugText("  (V4={} V5={} V6={} V7={})",
            m_CPU.m_Registers[RegisterID::V4],
            m_CPU.m_Registers[RegisterID::V5],
            m_CPU.m_Registers[RegisterID::V6],
            m_CPU.m_Registers[RegisterID::V7]
        );
        renderer.AddDebugText("  (V8={} V9={} VA={} VB={})",
            m_CPU.m_Registers[RegisterID::V8],
            m_CPU.m_Registers[RegisterID::V9],
            m_CPU.m_Registers[RegisterID::VA],
            m_CPU.m_Registers[RegisterID::VB]
        );
        renderer.AddDebugText("  (VC={} VD={} VE={} VF={})",
            m_CPU.m_Registers[RegisterID::VC],
            m_CPU.m_Registers[RegisterID::VD],
            m_CPU.m_Registers[RegisterID::VE],
            m_CPU.m_Registers[RegisterID::VF]
        );

        renderer.AddDebugText("- Index register: {}", m_CPU.m_Idx);
        renderer.AddDebugText("- Program counter: {}", m_CPU.m_PC);

        renderer.AddDebugText("- Delay timer: {}", m_CPU.m_DT);
        renderer.AddDebugText("- Sound timer: {}", m_CPU.m_ST);

        renderer.AddDebugText("- Keypad:");
        renderer.AddDebugText("  (K1={}, K2={}, K3={}, KC={})",
            m_CPU.m_Keypad[0x1],
            m_CPU.m_Keypad[0x2],
            m_CPU.m_Keypad[0x3],
            m_CPU.m_Keypad[0xC]
        );
        renderer.AddDebugText("  (K4={}, K5={}, K6={}, KD={})",
            m_CPU.m_Keypad[0x4],
            m_CPU.m_Keypad[0x5],
            m_CPU.m_Keypad[0x6],
            m_CPU.m_Keypad[0xD]
        );
        renderer.AddDebugText("  (K7={}, K8={}, K9={}, KE={})",
            m_CPU.m_Keypad[0x7],
            m_CPU.m_Keypad[0x8],
            m_CPU.m_Keypad[0x9],
            m_CPU.m_Keypad[0xE]
        );
        renderer.AddDebugText("  (KA={}, K0={}, KB={}, KF={})",
            m_CPU.m_Keypad[0xA],
            m_CPU.m_Keypad[0x0],
            m_CPU.m_Keypad[0xB],
            m_CPU.m_Keypad[0xF]
        );
    }
}

// --- static function implementations ----------------------------------------

void Chip8::ProcessInput() noexcept
{
    constexpr Key keys[C8_NUM_KEYS] = {
        Key::C8_KEY_0, Key::C8_KEY_1, Key::C8_KEY_2, Key::C8_KEY_3,
        Key::C8_KEY_4, Key::C8_KEY_5, Key::C8_KEY_6, Key::C8_KEY_7,
        Key::C8_KEY_8, Key::C8_KEY_9, Key::C8_KEY_A, Key::C8_KEY_B,
        Key::C8_KEY_C, Key::C8_KEY_D, Key::C8_KEY_E, Key::C8_KEY_F,
    };

    for (u8 k = 0; k < C8_NUM_KEYS; k++)
    {
        if (::IsKeyDown(static_cast<KeyboardKey>(keys[k])))
        {
            m_CPU.SetKey(k, 1);
        }
        else
        {
            m_CPU.SetKey(k, 0);
        }
    }
}

}
