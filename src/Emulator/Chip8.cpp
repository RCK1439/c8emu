#include "Chip8.hpp"
#include "Keyboard.hpp"

#include "Core/Debug.hpp"

#include "Renderer/Renderer.hpp"

namespace c8emu {

bool Chip8::LoadROM(const std::filesystem::path& filePath) noexcept
{
    if (!m_ROM.Load(filePath))
    {
        C8_LOG_ERROR("Failed to load ROM: {}", filePath.string());
        return false;
    }
    
    m_RAM.LoadROM(m_ROM);
    m_ROMLoaded = true;

    return true;
}

void Chip8::OnEvent(const sf::Event& event) noexcept
{
    if (const auto keyPress = event.getIf<sf::Event::KeyPressed>())
    {
        for (u8 k{}; k < C8_NUM_KEYS; k++)
            if (keyPress->code == C8_KEYS[k])
                m_CPU.SetKey(k, 1);
    }
    else if (const auto keyRelease = event.getIf<sf::Event::KeyReleased>())
    {
        for (u8 k{}; k < C8_NUM_KEYS; k++)
            if (keyRelease->code == C8_KEYS[k])
                m_CPU.SetKey(k, 0);
    }
}

void Chip8::OnUpdate(float dt) noexcept
{
    if (m_Tick >= C8_TICK_RATE)
    {
        if (m_ROMLoaded)
            m_CPU.Step(m_RAM);

        m_Tick -= C8_TICK_RATE;
    }

    m_Tick += dt;
}

void Chip8::OnRender(RenderContext& ctx) const noexcept
{
    const CPUData& cpuData = m_CPU.GetData();
    ctx.DrawBuffer(cpuData.Video.data(), C8_SCREEN_BUFFER_WIDTH<size_t>, C8_SCREEN_BUFFER_HEIGHT<size_t>);

    if (ctx.DebugOverlayEnabled())
    {
        ctx.AddDebugText("CPU:");
        ctx.AddDebugText(" REGISTERS:");
        ctx.AddDebugText("  V0:{} V1:{} V2:{} V3:{}",
            cpuData.Registers[RegisterID::V0],
            cpuData.Registers[RegisterID::V1],
            cpuData.Registers[RegisterID::V2],
            cpuData.Registers[RegisterID::V3]
        );
        ctx.AddDebugText("  V4:{} V5:{} V6:{} V7:{}",
            cpuData.Registers[RegisterID::V4],
            cpuData.Registers[RegisterID::V5],
            cpuData.Registers[RegisterID::V6],
            cpuData.Registers[RegisterID::V7]
        );
        ctx.AddDebugText("  V8:{} V9:{} VA:{} VB:{}",
            cpuData.Registers[RegisterID::V8],
            cpuData.Registers[RegisterID::V9],
            cpuData.Registers[RegisterID::VA],
            cpuData.Registers[RegisterID::VB]
        );
        ctx.AddDebugText("  VC:{} VD:{} VE:{} VF:{}",
            cpuData.Registers[RegisterID::VC],
            cpuData.Registers[RegisterID::VD],
            cpuData.Registers[RegisterID::VE],
            cpuData.Registers[RegisterID::VF]
        );

        ctx.AddDebugText(" INDEX REGISTER: {}", cpuData.Idx);
        ctx.AddDebugText(" PROGRAM COUNTER: {}", cpuData.PC);

        ctx.AddDebugText(" DELAY TIMER: {}", cpuData.DT);
        ctx.AddDebugText(" SOUND TIMER: {}", cpuData.ST);

        ctx.AddDebugText(" KEYPAD:");
        ctx.AddDebugText("  K1:{} K2:{} K3:{} KC:{}",
            cpuData.Keypad[0x1],
            cpuData.Keypad[0x2],
            cpuData.Keypad[0x3],
            cpuData.Keypad[0xC]
        );
        ctx.AddDebugText("  K4:{} K5:{} K6:{} KD:{}",
            cpuData.Keypad[0x4],
            cpuData.Keypad[0x5],
            cpuData.Keypad[0x6],
            cpuData.Keypad[0xD]
        );
        ctx.AddDebugText("  K7:{} K8:{} K9:{} KE:{}",
            cpuData.Keypad[0x7],
            cpuData.Keypad[0x8],
            cpuData.Keypad[0x9],
            cpuData.Keypad[0xE]
        );
        ctx.AddDebugText("  KA:{} K0:{} KB:{} KF:{}",
            cpuData.Keypad[0xA],
            cpuData.Keypad[0x0],
            cpuData.Keypad[0xB],
            cpuData.Keypad[0xF]
        );
    }
}

}
