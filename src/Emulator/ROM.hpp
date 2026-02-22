#pragma once

#include "Core/Types.hpp"

#include <filesystem>

namespace c8emu {

class ROM final
{
public:
    constexpr ROM() noexcept = default;
    ~ROM() noexcept;

    [[nodiscard]] bool Load(std::filesystem::path filePath) noexcept;

    [[nodiscard]] constexpr const Byte* Data() const noexcept { return m_Data; }
    [[nodiscard]] constexpr size_t Size() const noexcept { return m_Size; }
    [[nodiscard]] constexpr std::string_view Name() const noexcept { return m_Name; }
    [[nodiscard]] constexpr bool IsLoaded() const noexcept { return m_IsLoaded; }

private:
    std::string m_Name{};
    Byte*       m_Data{};
    size_t      m_Size{};
    bool        m_IsLoaded{};
};

}
