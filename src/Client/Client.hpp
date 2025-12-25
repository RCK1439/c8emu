#pragma once

#include "Core/Types.hpp"

#include "Emulator/Chip8.hpp"
#include "Emulator/ROM.hpp"

#include "Renderer/Renderer.hpp"

namespace c8emu {

class Client final
{
public:
    Client(i32 argc, char** argv) noexcept;
    ~Client() noexcept;

    void Run() noexcept;

private:
    void OnUpdate() noexcept;
    void OnRender() noexcept;
    void OnResize() noexcept;

private:
    Chip8    m_Chip8;
    Renderer m_Renderer;
    ROM      m_ROM;
    float    m_UpdateTime = 0.0f;
    float    m_RenderTime = 0.0f;
    bool     m_IsRunning = true;
};

}
