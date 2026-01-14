#pragma once

#include "Core/Types.hpp"

#include "Emulator/Chip8.hpp"
#include "Emulator/ROM.hpp"

#include "Renderer/Renderer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace c8emu {

class Client final
{
public:
    Client(i32 argc, char** argv) noexcept;
    ~Client() noexcept;

    void Run() noexcept;

private:
    void OnEvent(const sf::Event& event) noexcept;
    void OnUpdate() noexcept;
    void OnRender() noexcept;
    void OnResize(sf::Vector2u newSize) noexcept;

private:
    Chip8            m_Chip8{};
    Renderer         m_Renderer{};
    sf::RenderWindow m_Window{};
    ROM              m_ROM{};
    float            m_UpdateTime{};
    float            m_RenderTime{};
    float            m_DeltaTime{};
    bool             m_IsRunning{};
};

}
