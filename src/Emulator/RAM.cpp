#include "RAM.hpp"

#include <cstring>

namespace c8emu {

void RAM::LoadROM(const ROM& rom)
{
    const Buffer<Byte>& data = rom.Data();
    std::memcpy(m_Buffer.data() + C8_ADDR_ROM, data.Get(), rom.Size());
}

}
