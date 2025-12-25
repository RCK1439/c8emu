#include "Renderer.hpp"

#include "Core/Debug.hpp"

#include <raylib.h>

namespace c8emu {

constexpr Color C8_BG_COLOR = Color { .r = 0,   .g = 0,   .b = 255, .a = 255 };
constexpr Color C8_FG_COLOR = Color { .r = 255, .g = 255, .b = 255, .a = 255 };

void Renderer::Init(std::size_t width, std::size_t height) noexcept
{
    C8_ASSERT(::IsWindowReady(), "Window not yet ready");

    m_Font = ::GetFontDefault();
    m_Target = ::LoadRenderTexture(static_cast<i32>(width), static_cast<i32>(height));
    if (m_Target.id == 0)
    {
        Panic(ErrorCode::FAILED_TO_LOAD_TARGET, "Failed to load render target");
    }
    
    m_DrawDebugOverlay = false;
    m_Scale = static_cast<float>(::GetScreenWidth()) / static_cast<float>(width);
}

void Renderer::Shutdown() noexcept
{
    ::UnloadRenderTexture(m_Target);
}

void Renderer::Begin() noexcept
{
    ::BeginTextureMode(m_Target);
    ::ClearBackground(C8_BG_COLOR);
}

void Renderer::End() noexcept
{
    ::EndTextureMode();

    ::BeginDrawing();
    ::ClearBackground(C8_BG_COLOR);

    const Rectangle src = {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(m_Target.texture.width),
        .height = static_cast<float>(-m_Target.texture.height)
    };
    const Rectangle dest = {
        .x = 0.0f,
        .y = (static_cast<float>(::GetScreenHeight()) - static_cast<float>(m_Target.texture.height) * m_Scale) * 0.5f,
        .width = static_cast<float>(m_Target.texture.width) * m_Scale,
        .height = static_cast<float>(m_Target.texture.height) * m_Scale
    };
    const Vector2 origin = { .x = 0.0f, .y = 0.0f };
    ::DrawTexturePro(m_Target.texture, src, dest, origin, 0.0f, WHITE);
    DrawDebugOverlay();

    ::EndDrawing();
}

void Renderer::OnResize() noexcept
{
    m_Scale = static_cast<float>(::GetScreenWidth()) / static_cast<float>(m_Target.texture.width);
    C8_LOG_WARNING("Framebuffer scale adjusted: {:.2f}", m_Scale);
}

void Renderer::DrawBuffer(const u8* buffer, std::size_t width, std::size_t height) const noexcept
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

            ::DrawPixel(static_cast<i32>(x), static_cast<i32>(y), C8_FG_COLOR);
        }
    }
}

void Renderer::ToggleDebugOverlay() noexcept
{
    m_DrawDebugOverlay = !m_DrawDebugOverlay;
}

bool Renderer::DebugOverlayEnabled() const noexcept
{
    return m_DrawDebugOverlay;
}

void Renderer::DrawDebugOverlay() noexcept
{
    for (const DebugText& debugText : m_DebugOverlay)
    {
        const std::string_view text = debugText.Text();
        const Vector2 textSize = ::MeasureTextEx(m_Font, text.data(), 20.0f, 2.0f);
        const Vector2 position = debugText.Position();
        
        const Rectangle box = {
            .x = position.x - 5.0f,
            .y = position.y - 5.0f,
            .width = textSize.x + 10.0f,
            .height = textSize.y + 5.0f,
        };
        const Color boxColor = {
            .r = 0,
            .g = 0,
            .b = 0,
            .a = 128
        };

        ::DrawRectangleRec(box, boxColor);
        ::DrawTextEx(m_Font, text.data(), position, 20.0f, 2.0f, WHITE);
    }

    m_DebugOverlay.Clear();
}

}
