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

    [[nodiscard]] constexpr Byte& operator[](Address addr)
    {
        addr &= 0x0FFF;
        return m_Buffer.at(static_cast<size_t>(addr));
    }

    [[nodiscard]] constexpr const Byte& operator[](Address addr) const
    {
        addr &= 0x0FFF;
        return m_Buffer.at(static_cast<size_t>(addr));
    }

private:
    void LoadFont();

private:
    using MemoryBuffer = std::array<Byte, C8_MEMORY_SIZE>;
    
    MemoryBuffer m_Buffer{};
};

}
