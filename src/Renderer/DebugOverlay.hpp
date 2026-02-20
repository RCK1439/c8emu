#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

#include <format>
#include <vector>

namespace c8emu {

constexpr sf::Vector2f INIT_POSITION = { 5.0f, 5.0f };

struct DebugText final
{
public:
    sf::String   Text{};
    sf::Vector2f Position{};

public:
    constexpr DebugText(sf::String&& text, sf::Vector2f position) noexcept :
        Text(text),
        Position(position) {}
};

class DebugOverlay final
{
public:
    template<typename T>
    static constexpr T FONT_SIZE = static_cast<T>(16);
    template<typename T>
    static constexpr T FONT_SPACING = static_cast<T>(8);

    using ConstIter = std::vector<DebugText>::const_iterator;
    using Iter = std::vector<DebugText>::iterator;

    using RevConstIter = std::vector<DebugText>::const_reverse_iterator;
    using RevIter = std::vector<DebugText>::reverse_iterator;

public:
    constexpr DebugOverlay() = default;
    constexpr ~DebugOverlay() = default;

    constexpr void Clear() noexcept
    {
        m_Buffer.clear();
        m_NextPosition = INIT_POSITION;
    }

    template<typename ... Args>
    constexpr void Append(std::format_string<Args...> fmt, Args&& ... args) noexcept
    {
        m_Buffer.emplace_back(std::format(fmt, std::forward<Args>(args)...), m_NextPosition);
        m_NextPosition.y += FONT_SIZE<float> + FONT_SPACING<float>;
    }
    
    [[nodiscard]] constexpr size_t Size() const noexcept { return m_Buffer.size(); }

    [[nodiscard]] constexpr ConstIter cbegin() const { return m_Buffer.cbegin(); }
    [[nodiscard]] constexpr ConstIter cend() const { return m_Buffer.cend(); }

    [[nodiscard]] constexpr Iter begin() { return m_Buffer.begin(); }
    [[nodiscard]] constexpr Iter end() { return m_Buffer.end(); }

    [[nodiscard]] constexpr RevConstIter crbegin() const { return m_Buffer.crbegin(); }
    [[nodiscard]] constexpr RevConstIter crend() const { return m_Buffer.crend(); }

    [[nodiscard]] constexpr RevConstIter rbegin() const { return m_Buffer.rbegin(); }
    [[nodiscard]] constexpr RevConstIter rend() const { return m_Buffer.rend(); }

private:
    std::vector<DebugText> m_Buffer{};
    sf::Vector2f           m_NextPosition{INIT_POSITION};
};

}

