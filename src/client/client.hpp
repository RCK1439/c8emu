#pragma once

#include "core/types.hpp"

#include "emulator/chip8.hpp"
#include "emulator/rom.hpp"

#include "renderer/renderer.hpp"

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
