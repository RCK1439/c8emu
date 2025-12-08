#include "client.hpp"
#include "config.hpp"

#include "core/debug.hpp"

#include "emulator/chip8.hpp"
#include "emulator/specifications.hpp"

#include "renderer/renderer.hpp"

#include <raylib.h>

namespace c8emu {

Client::Client(i32 argc, char** argv) noexcept
{
    ::SetTraceLogLevel(LOG_NONE);
    C8_INIT_LOGGING();

    if (argc < 2)
    {
        C8_LOG_WARNING("usage: {} <rom_file>", argv[0]);
    }

    ::SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    ::InitWindow(C8_WINDOW_WIDTH<i32>, C8_WINDOW_HEIGHT<i32>, C8_WINDOW_TITLE);
    ::InitAudioDevice();

    ::SetWindowMinSize(C8_WINDOW_WIDTH<i32>, C8_WINDOW_HEIGHT<i32>);

    m_Renderer.Init(C8_SCREEN_BUFFER_WIDTH, C8_SCREEN_BUFFER_HEIGHT);

    if (argc > 1)
    {
        if (!m_ROM.Load(argv[1]))
        {
            C8_LOG_ERROR("Failed to load ROM: {}", argv[1]);
        }

        m_Chip8.LoadROM(m_ROM);
        ::SetWindowTitle(::TextFormat("%s - %s", C8_WINDOW_TITLE, m_ROM.Name().data()));
    }
}

Client::~Client() noexcept
{   
    ::CloseAudioDevice();
    ::CloseWindow();
    m_Renderer.Shutdown();
}

void Client::Run() noexcept
{
    while (m_IsRunning)
    {
        OnUpdate();
        OnRender();
    }
}

void Client::OnUpdate() noexcept
{
    const float t0 = static_cast<float>(::GetTime());
    if (::IsKeyPressed(KEY_F3))
    {
        m_Renderer.ToggleDebugOverlay();
    }

    if (::IsKeyPressed(KEY_F11))
    {
        if (!::IsWindowFullscreen())
        {
            const i32 monitorHandle = ::GetCurrentMonitor();
            const i32 monitorWidth = ::GetMonitorWidth(monitorHandle);
            const i32 monitorHeight = ::GetMonitorHeight(monitorHandle);
            C8_LOG_WARNING("monitor ({}) resolution: {}x{}", monitorHandle, monitorWidth, monitorHeight);

            ::SetWindowSize(monitorWidth, monitorHeight);
        }
        
        ::ToggleFullscreen();
        OnResize();

        C8_LOG_WARNING("Window resized to {}x{}", ::GetScreenWidth(), ::GetScreenHeight());
    }
    else if (::IsWindowResized())
    {
        OnResize();
        C8_LOG_WARNING("Window resized to {}x{}", ::GetScreenWidth(), ::GetScreenHeight());
    }

    m_Chip8.OnUpdate();

    m_IsRunning = !::WindowShouldClose();
    m_UpdateTime = static_cast<float>(::GetTime()) - t0;
}

void Client::OnRender() noexcept
{
    const float t0 = static_cast<float>(::GetTime());

    if (m_Renderer.DebugOverlayEnabled())
    {
        m_Renderer.AddDebugText("Client:");
        m_Renderer.AddDebugText("- Resolution: {}x{}", ::GetScreenWidth(), ::GetScreenHeight());
        m_Renderer.AddDebugText("- {} FPS ({:.2f}ms)", ::GetFPS(), ::GetFrameTime() * 1000.0f);
        m_Renderer.AddDebugText("- Update time: {:.5f}ms", m_UpdateTime * 1000.0f);
        m_Renderer.AddDebugText("- Render time: {:.5f}ms", m_RenderTime * 1000.0f);
    }

    m_Renderer.Begin(); 
        m_Chip8.OnRender(m_Renderer);
    m_Renderer.End();

    m_RenderTime = static_cast<float>(::GetTime()) - t0;
}

void Client::OnResize() noexcept
{
    m_Renderer.OnResize();
}

}
