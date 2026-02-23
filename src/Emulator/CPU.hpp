#pragma once

#include "Spec.hpp"
#include "CallStack.hpp"

#include <array>

namespace c8emu {

// Forward decleration
class RAM;

enum class RegisterID : u8
{
    V0 = 0x00,
    V1 = 0x01,
    V2 = 0x02,
    V3 = 0x03,
    V4 = 0x04,
    V5 = 0x05,
    V6 = 0x06,
    V7 = 0x07,
    V8 = 0x08,
    V9 = 0x09,
    VA = 0x0A,
    VB = 0x0B,
    VC = 0x0C,
    VD = 0x0D,
    VE = 0x0E,
    VF = 0x0F,
};

class Registers final
{
public:
    constexpr u8& operator[](u8 idx) noexcept { return m_Registers[static_cast<size_t>(idx)]; }
    constexpr const u8& operator[](u8 idx) const noexcept { return m_Registers[static_cast<size_t>(idx)]; }

    constexpr u8& operator[](RegisterID id) noexcept { return m_Registers[static_cast<size_t>(id)]; }
    constexpr const u8& operator[](RegisterID id) const noexcept { return m_Registers[static_cast<size_t>(id)]; }

private:
    using RegisterBuffer = std::array<u8, C8_NUM_REGISTERS>;
    
    RegisterBuffer m_Registers{};
};

struct CPUData
{
public:
    using VideoBuffer = std::array<Byte, C8_SCREEN_BUFFER_WIDTH<size_t> * C8_SCREEN_BUFFER_HEIGHT<size_t>>;
    using KeyPad      = std::array<u8, C8_NUM_KEYS>;

public:
    VideoBuffer Video{};
    CallStack   CallStack{};
    KeyPad      Keypad{};
    Registers   Registers{};
    u16         Idx{};
    u16         PC{C8_ADDR_PC};
    u8          DT{};
    u8          ST{};
};

class CPU
{
public:
    constexpr CPU() = default;
    
    void Step(RAM& ram) noexcept;
    void SetKey(u8 key, u8 val) noexcept;

    [[nodiscard]] inline const CPUData& GetData() const { return m_Data; }

private:
    CPUData m_Data{};
};

}
