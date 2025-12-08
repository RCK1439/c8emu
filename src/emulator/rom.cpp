#include "rom.hpp"
#include "specifications.hpp"

#include "core/debug.hpp"

#include <fstream>

namespace c8emu {

bool ROM::Load(std::filesystem::path filePath) noexcept
{
    std::ifstream rom(filePath, std::ios::in | std::ios::binary);
    if (!rom.is_open())
    {
        C8_LOG_ERROR("Couldn't open file: {}", filePath.string());
        return false;
    }

    rom.seekg(0, std::ios::end);
    m_Size = rom.tellg();
    if (m_Size > C8_MAX_ROM_SIZE)
    {
        C8_LOG_ERROR("ROM file too large!");
        return false;
    }
    rom.seekg(0, std::ios::beg);

    m_Data = new u8[m_Size];
    rom.read(reinterpret_cast<char*>(m_Data), sizeof(u8) * m_Size);
    
    m_Name = filePath.filename().replace_extension().string();
    C8_LOG_INFO("ROM successfully loaded {} bytes: {}", m_Size, filePath.filename().string());
    return true;
}

ROM::~ROM() noexcept
{
    delete[] m_Data;
}

}
