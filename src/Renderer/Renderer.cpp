#include "Renderer.hpp"

#include "Core/Debug.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace c8emu {

constexpr sf::Color C8_BG_COLOR = { 0,   0,   255, 255 };
constexpr sf::Color C8_FG_COLOR = { 255, 255, 255, 255 };

void Renderer::Init(const sf::RenderWindow& window, std::size_t width, std::size_t height) noexcept
{
    C8_ASSERT(window.isOpen(), "Window not yet open");

    if (!m_Font.openFromFile("resources/font/nintendo-nes-font.otf"))
    {
        Panic(ErrorCode::FAILED_TO_OPEN_FILE, "Failed to load font");
    }
    
    const sf::Vector2u frameBufferSize = { static_cast<u32>(width), static_cast<u32>(height) };
    if (!m_Target.resize(frameBufferSize))
    {
        Panic(ErrorCode::FAILED_TO_LOAD_TARGET, "Failed to load render target");
    }
    
    m_DrawDebugOverlay = false;
    m_Scale = static_cast<float>(window.getSize().x) / static_cast<float>(width);
}

void Renderer::Shutdown() noexcept
{
    // TODO: Anything?
}

RenderContext Renderer::Begin() noexcept
{
    m_Target.clear(C8_BG_COLOR);
    return RenderContext(m_Target, m_DebugOverlay, m_DrawDebugOverlay);
}

void Renderer::End(RenderContext& ctx, sf::RenderWindow& window) noexcept
{
    ctx.~RenderContext();

    window.clear();

    // const Rectangle src = {
    //     .x = 0.0f,
    //     .y = 0.0f,
    //     .width = static_cast<float>(m_Target.texture.width),
    //     .height = static_cast<float>(-m_Target.texture.height)
    // };
    // const Rectangle dest = {
    //     .x = 0.0f,
    //     .y = (static_cast<float>(::GetScreenHeight()) - static_cast<float>(m_Target.texture.height) * m_Scale) * 0.5f,
    //     .width = static_cast<float>(m_Target.texture.width) * m_Scale,
    //     .height = static_cast<float>(m_Target.texture.height) * m_Scale
    // };
    // const Vector2 origin = { .x = 0.0f, .y = 0.0f };
    // ::DrawTexturePro(m_Target.texture, src, dest, origin, 0.0f, WHITE);

    const sf::Texture& frameBuffer = m_Target.getTexture();
    const sf::Vector2u frameBufferSize = frameBuffer.getSize();
    const sf::Vector2f scale = {
        static_cast<float>(frameBufferSize.x) * m_Scale,
        static_cast<float>(frameBufferSize.y) * m_Scale,
    };
    const sf::Vector2f position = {
        0.0f,
        (static_cast<float>(window.getSize().y) - static_cast<float>(frameBufferSize.y) * m_Scale) * 0.5f,
    };

    sf::Sprite sprite(frameBuffer);
    sprite.setPosition(position);
    sprite.setScale(scale);
    sprite.setOrigin({ 0.0f, 0.0f });
    
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

void RenderContext::DrawBuffer(const u8* buffer, std::size_t width, std::size_t height) const noexcept
{
    for (std::size_t y = 0; y < height; y++)
    {
        for (std::size_t x = 0; x < width; x++)
        {
            const std::size_t idx = x + y * width;
            if (!buffer[idx])
            {
                continue;
            }

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

void Renderer::ToggleDebugOverlay() noexcept
{
    m_DrawDebugOverlay = !m_DrawDebugOverlay;
}

void Renderer::DrawDebugOverlay(sf::RenderWindow& window) noexcept
{
    for (const DebugText& debugText : m_DebugOverlay)
    {
        // const std::string_view text = debugText.Text();
        // const Vector2 textSize = ::MeasureTextEx(m_Font, text.data(), 20.0f, 2.0f);
        // const Vector2 position = debugText.Position();
        // const Rectangle box = {
        //     .x = position.x - 5.0f,
        //     .y = position.y - 5.0f,
        //     .width = textSize.x + 10.0f,
        //     .height = textSize.y + 5.0f,
        // };
        // const Color boxColor = {
        //     .r = 0,
        //     .g = 0,
        //     .b = 0,
        //     .a = 128
        // };
        // ::DrawRectangleRec(box, boxColor);
        // ::DrawTextEx(m_Font, text.data(), position, 20.0f, 2.0f, WHITE);

        const sf::String string = debugText.Text().data();
        const sf::Vector2f position = debugText.Position();
        
        sf::Text text(m_Font, string, 24);
        text.setPosition(position);
        
        // const sf::Vector2f textScale = text.getScale();
        // sf::RectangleShape box({ 16.0f*textScale.x + 10.0f, 16.0f*textScale.y + 5.0f });
        // box.setPosition({ position.x - 5.0f, position.y - 5.0f });
        // box.setFillColor(sf::Color(0, 0, 0, 128));

        // window.draw(box);
        window.draw(text);
    }

    m_DebugOverlay.Clear();
}

}
