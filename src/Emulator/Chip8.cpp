#include "Chip8.hpp"
#include "CPU.hpp"
#include "Keyboard.hpp"
#include "RAM.hpp"
#include "ROM.hpp"

#include "Core/Types.hpp"

#include "Renderer/Renderer.hpp"

namespace c8emu {

void Chip8::LoadROM(const ROM& rom) noexcept
{
    m_RAM.LoadROM(rom);
    m_ROMLoaded = true;
}

void Chip8::OnEvent(const sf::Event& event) noexcept
{
    if (const auto keyPress = event.getIf<sf::Event::KeyPressed>())
    {
        for (u8 k{}; k < C8_NUM_KEYS; k++)
        {
            if (keyPress->code == C8_KEYS[k])
            {
                m_CPU.SetKey(k, 1);
            }
        }
    }
    else if (const auto keyRelease = event.getIf<sf::Event::KeyReleased>())
    {
        for (u8 k{}; k < C8_NUM_KEYS; k++)
        {
            if (keyRelease->code == C8_KEYS[k])
            {
                m_CPU.SetKey(k, 0);
            }
        }
    }
}

void Chip8::OnUpdate(float dt) noexcept
{
    if (m_Tick >= C8_TICK_RATE)
    {
        if (m_ROMLoaded)
        {
            m_CPU.Step(m_RAM);
        }

        m_Tick -= C8_TICK_RATE;
    }

    m_Tick += dt;
}

void Chip8::OnRender(RenderContext& ctx) const noexcept
{
    ctx.DrawBuffer(m_CPU.m_Video.data(), C8_SCREEN_BUFFER_WIDTH, C8_SCREEN_BUFFER_HEIGHT);

    if (ctx.DebugOverlayEnabled())
    {
        ctx.AddDebugText("CPU:");
        ctx.AddDebugText(" REGISTERS:");
        ctx.AddDebugText("  V0:{} V1:{} V2:{} V3:{}",
            m_CPU.m_Registers[RegisterID::V0],
            m_CPU.m_Registers[RegisterID::V1],
            m_CPU.m_Registers[RegisterID::V2],
            m_CPU.m_Registers[RegisterID::V3]
        );
        ctx.AddDebugText("  V4:{} V5:{} V6:{} V7:{}",
            m_CPU.m_Registers[RegisterID::V4],
            m_CPU.m_Registers[RegisterID::V5],
            m_CPU.m_Registers[RegisterID::V6],
            m_CPU.m_Registers[RegisterID::V7]
        );
        ctx.AddDebugText("  V8:{} V9:{} VA:{} VB:{}",
            m_CPU.m_Registers[RegisterID::V8],
            m_CPU.m_Registers[RegisterID::V9],
            m_CPU.m_Registers[RegisterID::VA],
            m_CPU.m_Registers[RegisterID::VB]
        );
        ctx.AddDebugText("  VC:{} VD:{} VE:{} VF:{}",
            m_CPU.m_Registers[RegisterID::VC],
            m_CPU.m_Registers[RegisterID::VD],
            m_CPU.m_Registers[RegisterID::VE],
            m_CPU.m_Registers[RegisterID::VF]
        );

        ctx.AddDebugText(" INDEX REGISTER: {}", m_CPU.m_Idx);
        ctx.AddDebugText(" PROGRAM COUNTER: {}", m_CPU.m_PC);

        ctx.AddDebugText(" DELAY TIMER: {}", m_CPU.m_DT);
        ctx.AddDebugText(" SOUND TIMER: {}", m_CPU.m_ST);

        ctx.AddDebugText(" KEYPAD:");
        ctx.AddDebugText("  K1:{} K2:{} K3:{} KC:{}",
            m_CPU.m_Keypad[0x1],
            m_CPU.m_Keypad[0x2],
            m_CPU.m_Keypad[0x3],
            m_CPU.m_Keypad[0xC]
        );
        ctx.AddDebugText("  K4:{} K5:{} K6:{} KD:{}",
            m_CPU.m_Keypad[0x4],
            m_CPU.m_Keypad[0x5],
            m_CPU.m_Keypad[0x6],
            m_CPU.m_Keypad[0xD]
        );
        ctx.AddDebugText("  K7:{} K8:{} K9:{} KE:{}",
            m_CPU.m_Keypad[0x7],
            m_CPU.m_Keypad[0x8],
            m_CPU.m_Keypad[0x9],
            m_CPU.m_Keypad[0xE]
        );
        ctx.AddDebugText("  KA:{} K0:{} KB:{} KF:{}",
            m_CPU.m_Keypad[0xA],
            m_CPU.m_Keypad[0x0],
            m_CPU.m_Keypad[0xB],
            m_CPU.m_Keypad[0xF]
        );
    }
}

}
