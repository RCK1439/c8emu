#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

#include <cstddef>

#include <format>
#include <vector>

namespace c8emu {

constexpr sf::Vector2f INIT_POSITION = { 5.0f, 5.0f };

struct DebugText final
{
public:
    sf::String   m_Text{};
    sf::Vector2f m_Position{};

public:
    DebugText(sf::String&& text, sf::Vector2f position) noexcept :
        m_Text(text),
        m_Position(position) {}
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
    DebugOverlay() = default;
    ~DebugOverlay() = default;

    void Clear() noexcept
    {
        m_Buffer.clear();
        m_NextPosition = INIT_POSITION;
    }

    template<typename ... Args>
    void Append(std::format_string<Args...> fmt, Args&& ... args) noexcept
    {
        m_Buffer.emplace_back(std::format(fmt, std::forward<Args>(args)...), m_NextPosition);
        m_NextPosition.y += FONT_SIZE<float> + FONT_SPACING<float>;
    }
    
    [[nodiscard]] inline std::size_t Size() const noexcept { return m_Buffer.size(); }

    [[nodiscard]] inline ConstIter cbegin() const { return m_Buffer.cbegin(); }
    [[nodiscard]] inline ConstIter cend() const { return m_Buffer.cend(); }

    [[nodiscard]] inline Iter begin() { return m_Buffer.begin(); }
    [[nodiscard]] inline Iter end() { return m_Buffer.end(); }

    [[nodiscard]] inline RevConstIter crbegin() const { return m_Buffer.crbegin(); }
    [[nodiscard]] inline RevConstIter crend() const { return m_Buffer.crend(); }

    [[nodiscard]] inline RevConstIter rbegin() const { return m_Buffer.rbegin(); }
    [[nodiscard]] inline RevConstIter rend() const { return m_Buffer.rend(); }

private:
    std::vector<DebugText> m_Buffer{};
    sf::Vector2f           m_NextPosition{INIT_POSITION};
};

}

