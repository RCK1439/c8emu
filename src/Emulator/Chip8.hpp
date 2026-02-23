#pragma once

#include "CPU.hpp"
#include "RAM.hpp"
#include "ROM.hpp"

#include <SFML/Window/Event.hpp>

#include <filesystem>

namespace c8emu {

class RenderContext;

class Chip8 final
{
public:
    constexpr Chip8() noexcept = default;
    
    [[nodiscard]] bool LoadROM(const std::filesystem::path& filePath) noexcept;

    void OnEvent(const sf::Event& event) noexcept;
    void OnUpdate(float dt) noexcept;
    void OnRender(RenderContext& ctx) const noexcept;

    [[nodiscard]] inline const ROM& GetROM() const noexcept { return m_ROM; }

private:
    RAM   m_RAM{};
    CPU   m_CPU{};
    ROM   m_ROM{};
    float m_Tick{};
    bool  m_ROMLoaded{};
};

}
