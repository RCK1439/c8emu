#pragma once

#include "ram.hpp"
#include "cpu.hpp"

#include "renderer/renderer.hpp"

namespace c8emu {

class Chip8 final
{
public:
    Chip8() = default;
    ~Chip8() = default;

    void LoadROM(const ROM& rom) noexcept;

    void OnUpdate() noexcept;
    void OnRender(Renderer& renderer) const noexcept;

private:
    void ProcessInput() noexcept;

private:
    RAM   m_RAM;
    CPU   m_CPU;
    bool  m_ROMLoaded = false;
    float m_Tick = 0.0f;
};

}
