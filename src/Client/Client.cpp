#include "Client.hpp"
#include "Config.hpp"

#include "Core/Debug.hpp"

#include "Emulator/Chip8.hpp"
#include "Emulator/Spec.hpp"

#include "Renderer/Renderer.hpp"

#include <SFML/Window/VideoMode.hpp>
#include <chrono>

namespace c8emu {

Client::Client(i32 argc, char** argv) noexcept
{
    if (argc < 2)
    {
        C8_LOG_WARNING("usage: {} <rom_file>", argv[0]);
    }

    const sf::Vector2u windowSize(C8_WINDOW_WIDTH<u32>, C8_WINDOW_HEIGHT<u32>);
    const sf::Vector2u targetSize(C8_SCREEN_BUFFER_WIDTH<u32>, C8_SCREEN_BUFFER_HEIGHT<u32>);
    const sf::VideoMode videoMode(windowSize);
    
    m_Window.create(videoMode, C8_WINDOW_TITLE);
    m_Window.setVerticalSyncEnabled(true);
    m_Window.setMinimumSize(windowSize);

    m_Renderer.Init(windowSize, targetSize);

    if (argc > 1)
    {
        if (!m_ROM.Load(argv[1]))
        {
            C8_LOG_ERROR("Failed to load ROM: {}", argv[1]);
        }

        m_Chip8.LoadROM(m_ROM);
        m_Window.setTitle(std::format("{} - {}", C8_WINDOW_TITLE, m_ROM.Name().data()));
    }
}

Client::~Client() noexcept
{
    m_Window.close();
    m_Renderer.Shutdown();
}

void Client::Run() noexcept
{
    m_IsRunning = true;
    while (m_IsRunning)
    {
        const auto t0 = std::chrono::high_resolution_clock::now();
        while (const auto e = m_Window.pollEvent())
        {
            const sf::Event event = e.value();
            OnEvent(event);
        }

        OnUpdate();
        OnRender();

        const std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - t0;
        m_DeltaTime = elapsed.count();
    }
}

void Client::OnEvent(const sf::Event& event) noexcept
{
    if (const auto key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::F3)
        {
            m_Renderer.ToggleDebugOverlay();
        }
        else if (key->code == sf::Keyboard::Key::F11)
        {
            const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
            const bool fullScreen = desktopMode.size == m_Window.getSize();
            if (!fullScreen)
            {
                // TODO: Set window title to the current title
                m_Window.create(desktopMode, C8_WINDOW_TITLE, sf::Style::Default, sf::State::Fullscreen);
                m_Window.setVerticalSyncEnabled(true);
            }
            else
            {
                // TODO: Set window title to the current title
                m_Window.create(desktopMode, C8_WINDOW_TITLE, sf::Style::Default, sf::State::Windowed);
                m_Window.setVerticalSyncEnabled(true);
            }

            OnResize(desktopMode.size);
        }
        else if (key->code == sf::Keyboard::Key::Escape)
        {
            m_IsRunning = false;
        }
    }
    else if (const auto resizeData = event.getIf<sf::Event::Resized>())
    {
        const sf::Vector2u newSize = resizeData->size;
        OnResize(newSize);
    }

    m_Chip8.OnEvent(event);
}

void Client::OnUpdate() noexcept
{
    const auto t0 = std::chrono::high_resolution_clock::now();
    m_Chip8.OnUpdate(m_DeltaTime);

    const std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - t0;
    m_UpdateTime = elapsed.count();
}

void Client::OnRender() noexcept
{
    const auto t0 = std::chrono::high_resolution_clock::now();

    RenderContext ctx = m_Renderer.Begin();
    if (ctx.DebugOverlayEnabled())
    {
        const sf::Vector2u windowSize = m_Window.getSize();
        const i32 fps = static_cast<i32>(1.0f / m_DeltaTime);
        const float frameTime = m_DeltaTime * 1000.0f;

        ctx.AddDebugText("CLIENT:");
        ctx.AddDebugText(" {} FPS, {:.2f}MS", fps, frameTime);
        ctx.AddDebugText(" RESOLUTION: {}x{}", windowSize.x, windowSize.y);
        ctx.AddDebugText(" UPDATE TIME: {:.5f}MS", m_UpdateTime * 1000.0f);
        ctx.AddDebugText(" RENDER TIME: {:.5f}MS", m_RenderTime * 1000.0f);
    }
    m_Chip8.OnRender(ctx);
    m_Renderer.End(std::move(ctx), m_Window);

    const std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - t0;
    m_RenderTime = elapsed.count();
}

void Client::OnResize(sf::Vector2u newSize) noexcept
{
    m_Window.setSize(newSize);
    m_Renderer.OnResize(newSize);
    C8_LOG_WARNING("Window resized to {}x{}", newSize.x, newSize.y);
}

}
