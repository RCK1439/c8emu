#pragma once

#include <SFML/System/Vector2.hpp>

#include <cstddef>

#include <format>
#include <vector>
#include <string>

namespace c8emu {

constexpr sf::Vector2f INIT_POSITION = { 5.0f, 5.0f };

class DebugText final
{
public:
    DebugText(std::string&& text, sf::Vector2f position) noexcept :
        m_Text(text), m_Position(position) {}

    inline std::string_view Text() const noexcept { return m_Text; }
    inline sf::Vector2f Position() const noexcept { return m_Position; }

private:
    std::string  m_Text{};
    sf::Vector2f m_Position{};
};

class DebugOverlay final
{
public:
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
        m_NextPosition.y += 20.0f + 5.0f; // font size + spacing
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

