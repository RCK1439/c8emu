#pragma once

#include "RAM.hpp"
#include "CPU.hpp"

#include "Renderer/Renderer.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace c8emu {

class Chip8 final
{
public:
    Chip8() = default;
    ~Chip8() = default;

    void LoadROM(const ROM& rom) noexcept;

    void OnEvent(const sf::Event& event) noexcept;
    void OnUpdate(float dt) noexcept;
    void OnRender(RenderContext& ctx) const noexcept;

private:
    RAM   m_RAM{};
    CPU   m_CPU{};
    bool  m_ROMLoaded{};
    float m_Tick{};
};

}
