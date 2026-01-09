#pragma once

#include <raylib.h>

#include <cstddef>

#include <format>
#include <vector>
#include <string>

namespace c8emu {

constexpr Vector2     INIT_POSITION = Vector2 { 5.0f, 5.0f };

class DebugText final
{
public:
    DebugText(std::string&& text, Vector2 position) noexcept :
        m_Text(text), m_Position(position) {}

    inline std::string_view Text() const noexcept
    {
        return m_Text;
    }

    inline Vector2 Position() const noexcept
    {
        return m_Position;
    }

private:
    std::string m_Text{};
    Vector2     m_Position{};
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
    
    [[nodiscard]] inline std::size_t Size() const noexcept
    {
        return m_Buffer.size();
    }

    [[nodiscard]] inline ConstIter cbegin() const
    {
        return m_Buffer.cbegin();
    }

    [[nodiscard]] inline ConstIter cend() const
    {
        return m_Buffer.cend();
    }

    [[nodiscard]] inline Iter begin()
    {
        return m_Buffer.begin();
    }

    [[nodiscard]] inline Iter end()
    {
        return m_Buffer.end();
    }

    [[nodiscard]] inline RevConstIter crbegin() const
    {
        return m_Buffer.crbegin();
    }

    [[nodiscard]] inline RevConstIter crend() const
    {
        return m_Buffer.crend();
    }

    [[nodiscard]] inline RevConstIter rbegin() const
    {
        return m_Buffer.rbegin();
    }

    [[nodiscard]] inline RevConstIter rend() const
    {
        return m_Buffer.rend();
    }

private:
    std::vector<DebugText> m_Buffer{};
    Vector2                m_NextPosition{INIT_POSITION};
};

}

