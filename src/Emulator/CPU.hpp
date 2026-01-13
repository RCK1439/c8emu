#pragma once

#include "Instructions.hpp"
#include "RAM.hpp"
#include "Spec.hpp"
#include "CallStack.hpp"

#include "Core/Types.hpp"

#include <array>
#include <random>

namespace c8emu {

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
    constexpr u8& operator[](u8 idx) noexcept
    {
        return m_Registers.at(static_cast<std::size_t>(idx));
    }

    constexpr const u8& operator[](u8 idx) const noexcept
    {
        return m_Registers.at(static_cast<std::size_t>(idx));
    }

    constexpr u8& operator[](RegisterID id) noexcept
    {
        return m_Registers.at(static_cast<std::size_t>(id));
    }

    constexpr const u8& operator[](RegisterID id) const noexcept
    {
        return m_Registers.at(static_cast<std::size_t>(id));
    }

private:
    using RegisterBuffer = std::array<u8, C8_NUM_REGISTERS>;
    
    RegisterBuffer m_Registers{};
};

class CPU final
{
public:
    CPU() = default;
    
    void Step(RAM& ram) noexcept;
    void SetKey(u8 key, u8 val) noexcept;

private:
    using VideoBuffer = std::array<Byte, C8_SCREEN_BUFFER_WIDTH * C8_SCREEN_BUFFER_HEIGHT>;
    using KeyPad      = std::array<u8, C8_NUM_KEYS>;

    friend void Raw(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Cls(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Ret(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Jp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Call(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Se(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Sne(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Ld(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Add(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Or(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void And(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Xor(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Sub(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Shr(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Subn(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Shl(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Rnd(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Drw(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Skp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
    friend void Sknp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;

    friend class Chip8;

private:
    VideoBuffer  m_Video{};
    CallStack    m_CallStack{};
    KeyPad       m_Keypad{};
    Registers    m_Registers{};
    u16          m_Idx{};
    u16          m_PC{C8_ADDR_PC};
    u8           m_DT{};
    u8           m_ST{};
};

}
