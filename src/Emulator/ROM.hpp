#pragma once

#include "Core/Buffer.hpp"
#include "Core/Types.hpp"

#include <filesystem>

namespace c8emu {

class ROM final
{
public:
    constexpr ROM() noexcept = default;
    constexpr ~ROM() noexcept = default;

    [[nodiscard]] bool Load(const std::filesystem::path& filePath) noexcept;

    [[nodiscard]] constexpr const Buffer<Byte>& GetData() const noexcept { return m_Data; }
    [[nodiscard]] constexpr size_t GetSize() const noexcept { return m_Data.GetSize(); }
    [[nodiscard]] constexpr std::string_view GetName() const noexcept { return m_Name; }
    [[nodiscard]] constexpr bool IsLoaded() const noexcept { return m_IsLoaded; }

private:
    std::string  m_Name{};
    Buffer<Byte> m_Data{};
    bool         m_IsLoaded{};
};

}
