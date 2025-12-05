#pragma once

#include "debug_overlay.hpp"

#include "core/types.hpp"

#include <format>

namespace c8emu {

class Renderer final
{
public:
    Renderer() = default;
    ~Renderer() = default;

    void Init(std::size_t width, std::size_t height) noexcept;
    void Shutdown() noexcept;

    void Begin() noexcept;
    void End() noexcept;

    void OnResize() noexcept;

    void DrawBuffer(const u8* buffer, std::size_t width, std::size_t height) const noexcept;

    void ToggleDebugOverlay() noexcept;
    bool DebugOverlayEnabled() const noexcept;

    template<typename ... Args>
    void AddDebugText(std::format_string<Args...> fmt, Args&& ... args) noexcept
    {
        if (!m_DrawDebugOverlay)
        {
            return;
        }

        m_DebugOverlay.Append(fmt, std::forward<Args>(args)...);
    }

private:
    void DrawDebugOverlay() noexcept;

private:
    Font            m_Font;
    RenderTexture2D m_Target;
    DebugOverlay    m_DebugOverlay;
    bool            m_DrawDebugOverlay;
    float           m_Scale;
};

}
