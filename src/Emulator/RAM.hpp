#pragma once

#include "ROM.hpp"
#include "Spec.hpp"

#include <array>

namespace c8emu {

class RAM
{
public:
    RAM();

    void LoadROM(const ROM& rom);

    constexpr u8& operator[](u16 addr)
    {
        addr &= 0x0FFF;
        return m_Buffer.at(static_cast<std::size_t>(addr));
    }

    constexpr const u8& operator[](u16 addr) const
    {
        addr &= 0x0FFF;
        return m_Buffer.at(static_cast<std::size_t>(addr));
    }

private:
    void LoadFont();

private:
    using MemoryBuffer = std::array<u8, C8_MEMORY_SIZE>;
    
    MemoryBuffer m_Buffer = { 0 };
};

}
