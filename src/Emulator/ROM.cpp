#include "ROM.hpp"
#include "Spec.hpp"

#include "Core/Debug.hpp"

#include <fstream>

namespace c8emu {

bool ROM::Load(const std::filesystem::path& filePath) noexcept
{
    std::ifstream rom(filePath, std::ios::in | std::ios::binary);
    if (!rom.is_open())
    {
        C8_LOG_ERROR("Couldn't open file: {}", filePath.string());
        return false;
    }

    rom.seekg(0, std::ios::end);
    const size_t size = rom.tellg();
    if (size > C8_MAX_ROM_SIZE)
    {
        C8_LOG_ERROR("ROM file too large!");
        return false;
    }
    rom.seekg(0, std::ios::beg);

    m_Data.Reset(size);
    rom.read(reinterpret_cast<char*>(m_Data.GetMutablePtr()), sizeof(Byte) * size);
    
    m_Name = filePath.filename().replace_extension().string();
    C8_LOG_INFO("ROM successfully loaded {} bytes: {}", size, filePath.filename().string());
    return true;
}

}
