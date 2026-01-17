#pragma once

#include "DebugOverlay.hpp"

#include "Core/Types.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <format>

namespace c8emu {

struct RenderContext final
{
public:
    RenderContext() = delete;
    RenderContext(const RenderContext&) = delete;
    RenderContext(RenderContext&&) = delete;

    void DrawBuffer(const Byte* buffer, size_t width, size_t height) const noexcept;

    inline bool DebugOverlayEnabled() const noexcept { return m_DrawDebugOverlay; }

    template<typename ... Args>
    void AddDebugText(std::format_string<Args...> fmt, Args&& ... args) noexcept
    {
        if (!m_DrawDebugOverlay)
            return;

        m_DebugOverlay.Append(fmt, std::forward<Args>(args)...);
    }

private:
    friend class Renderer;

    explicit RenderContext(sf::RenderTexture& target, DebugOverlay& overlay, bool drawDebugOverlay) :
        m_Target(target),
        m_DebugOverlay(overlay),
        m_DrawDebugOverlay(drawDebugOverlay) {};

private:
    sf::RenderTexture& m_Target;
    DebugOverlay&      m_DebugOverlay;
    const bool         m_DrawDebugOverlay{};
};

class Renderer final
{
public:
    Renderer() = default;
    ~Renderer() = default;

    void Init(sf::Vector2u windowSize, sf::Vector2u targetSize) noexcept;
    void Shutdown() noexcept;

    [[nodiscard]] RenderContext Begin() noexcept;
    void End(RenderContext&& ctx, sf::RenderWindow& window) noexcept;

    void OnResize(sf::Vector2u newSize) noexcept;

    void ToggleDebugOverlay() noexcept;

private:
    void DrawDebugOverlay(sf::RenderWindow& window) noexcept;

private:
    sf::RenderTexture m_Target{};
    sf::Font          m_Font{};
    DebugOverlay      m_DebugOverlay{};
    float             m_Scale{};
    bool              m_DrawDebugOverlay{};
};

}
