#include "Renderer.hpp"

#include "Core/Debug.hpp"
#include "Core/NintendoNESFont.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace c8emu {

constexpr sf::Color C8_BG_COLOR = { 0,   0,   255, 255 };
constexpr sf::Color C8_FG_COLOR = { 255, 255, 255, 255 };

void RenderContext::DrawBuffer(const Byte* buffer, size_t width, size_t height) const noexcept
{
    for (size_t y{}; y < height; y++)
    {
        for (size_t x{}; x < width; x++)
        {
            const size_t idx = x + y * width;
            if (!buffer[idx])
                continue;

            const sf::Vector2f position = {
                static_cast<float>(x),
                static_cast<float>(y),
            };
            const sf::Vector2f size = { 1.0f, 1.0f };
            
            sf::RectangleShape px(size);
            px.setPosition(position);
            px.setFillColor(C8_FG_COLOR);

            m_Target.draw(px);
        }
    }
}

void Renderer::Init(sf::Vector2u windowSize, sf::Vector2u targetSize) noexcept
{
    if (!m_Font.openFromMemory(NINTENDO_NES_FONT_OTF, sizeof(NINTENDO_NES_FONT_OTF)))
        Panic(ErrorCode::FAILED_TO_OPEN_FILE, "Failed to load font");
    
    if (!m_Target.resize(targetSize))
        Panic(ErrorCode::FAILED_TO_LOAD_TARGET, "Failed to load render target");
    
    m_DrawDebugOverlay = false;
    m_Scale = static_cast<float>(windowSize.x) / static_cast<float>(targetSize.x);
}

void Renderer::Shutdown() noexcept
{
    // Anything? No, not really, but let's keep it anyway :)
}

RenderContext Renderer::Begin() noexcept
{
    m_Target.clear(C8_BG_COLOR);
    return RenderContext(m_Target, m_DebugOverlay, m_DrawDebugOverlay);
}

void Renderer::End(RenderContext&& ctx, sf::RenderWindow& window) noexcept
{
    ctx.~RenderContext();
    m_Target.display();

    const sf::Texture& frameBuffer = m_Target.getTexture();
    const sf::Vector2u frameBufferSize = frameBuffer.getSize();
    const sf::Vector2f scale = { m_Scale, m_Scale, };
    const sf::Vector2f position = {
        0.0f,
        (static_cast<float>(window.getSize().y) - static_cast<float>(frameBufferSize.y) * m_Scale) * 0.5f,
    };

    sf::Sprite sprite(frameBuffer);
    sprite.setPosition(position);
    sprite.setScale(scale);
    
    window.clear(C8_BG_COLOR);
    window.draw(sprite);
    DrawDebugOverlay(window);

    window.display();
}

void Renderer::OnResize(sf::Vector2u newSize) noexcept
{
    const float windowWidth = static_cast<float>(newSize.x);
    const float frameBufferWidth = static_cast<float>(m_Target.getSize().x);
    
    m_Scale = windowWidth / frameBufferWidth;
    C8_LOG_WARNING("Framebuffer scale adjusted: {:.2f}", m_Scale);
}

void Renderer::ToggleDebugOverlay() noexcept
{
    m_DrawDebugOverlay = !m_DrawDebugOverlay;
}

void Renderer::DrawDebugOverlay(sf::RenderWindow& window) noexcept
{
    constexpr sf::Vector2f PADDING = { 5.0f, 5.0f };
    constexpr sf::Color TEXT_BOX_COLOR = { 0, 0, 0, 128 };

    for (const auto& [string, position] : m_DebugOverlay)
    {
        sf::Text text(m_Font, string, DebugOverlay::FONT_SIZE<u32>);
        text.setPosition(position);
        
        const sf::FloatRect bounds = text.getLocalBounds();
        sf::RectangleShape box(bounds.size + 2.0f * PADDING);
        box.setPosition(position - PADDING);
        box.setFillColor(TEXT_BOX_COLOR);

        window.draw(box);
        window.draw(text);
    }

    m_DebugOverlay.Clear();
}

}
