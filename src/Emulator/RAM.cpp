#include "RAM.hpp"

#include <cstring>

namespace c8emu {

void RAM::LoadROM(const ROM& rom)
{
    std::memcpy(m_Buffer.data() + C8_ADDR_ROM, rom.Data(), rom.Size());
}

}
