#include "RAM.hpp"
#include "ROM.hpp"

#include <cstring>

namespace c8emu {

void RAM::LoadROM(const ROM& rom) noexcept
{
    const Buffer<Byte>& data = rom.GetData();
    std::memcpy(m_Buffer.data() + C8_ADDR_ROM, data.GetConstPtr(), rom.GetSize());
}

}
