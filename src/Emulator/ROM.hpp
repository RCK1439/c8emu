#pragma once

#include "Core/Types.hpp"

#include <filesystem>

namespace c8emu {

class ROM final
{
public:
    ROM() noexcept = default;
    ~ROM() noexcept;

    [[nodiscard]] bool Load(std::filesystem::path filePath) noexcept;

    [[nodiscard]] inline const Byte* Data() const noexcept { return m_Data; }
    [[nodiscard]] inline size_t Size() const noexcept { return m_Size; }
    [[nodiscard]] inline std::string_view Name() const noexcept { return m_Name; }
    [[nodiscard]] inline bool IsLoaded() const noexcept { return m_IsLoaded; }

private:
    std::string m_Name{};
    Byte*       m_Data{};
    size_t      m_Size{};
    bool        m_IsLoaded{};
};

}
