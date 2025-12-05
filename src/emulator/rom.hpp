#pragma once

#include "core/types.hpp"

#include <filesystem>

namespace c8emu {

class ROM final
{
public:
    ROM() noexcept = default;
    ~ROM() noexcept;

    bool Load(std::filesystem::path filePath) noexcept;

    inline const u8* Data() const noexcept
    {
        return m_Data;
    }

    inline std::size_t Size() const noexcept
    {
        return m_Size;
    }

    inline std::string_view Name() const noexcept
    {
        return m_Name;
    }

    inline bool IsLoaded() const noexcept
    {
        return m_IsLoaded;
    }

private:
    std::string m_Name;
    u8*         m_Data = nullptr;
    std::size_t m_Size = 0;
    bool        m_IsLoaded = false;
};

}
