#pragma once

#include "RAM.hpp"
#include "CPU.hpp"

#include "Renderer/Renderer.hpp"

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
    RAM   m_RAM{};
    CPU   m_CPU{};
    bool  m_ROMLoaded{};
    float m_Tick{};
};

}
