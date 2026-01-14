#pragma once

#include "Core/Types.hpp"

#include <cstddef>
#include <variant>

namespace c8emu {

enum class Instr : u8
{
    RAW,
    CLS,
    RET,
    JP,
    CALL,
    SE,
    SNE,
    LD,
    ADD,
    OR,
    AND,
    XOR,
    SUB,
    SHR,
    SUBN,
    SHL,
    RND,
    DRW,
    SKP,
    SKNP
};

enum class AddrMode : u32
{
    NONE      = 0,
    OPCODE    = 1 << 0,
    ADDR      = 1 << 1,
    VX_BYTE   = 1 << 2,
    VX_VY     = 1 << 3,
    I_ADDR    = 1 << 4,
    V0_ADDR   = 1 << 5,
    VX_VY_N   = 1 << 6,
    VX        = 1 << 7,
    VX_DT     = 1 << 8,
    VX_KEY    = 1 << 9,
    DT_VX     = 1 << 10,
    ST_VX     = 1 << 11,
    I_VX      = 1 << 12,
    FONT_VX   = 1 << 13,
    BCD_VX    = 1 << 14,
    ADDR_I_VX = 1 << 15,
    VX_ADDR_I = 1 << 16,
};

constexpr size_t operator|(AddrMode a, AddrMode b) noexcept
{
    return static_cast<size_t>(a) | static_cast<size_t>(b);
}

constexpr size_t operator&(AddrMode a, AddrMode b) noexcept
{
    return static_cast<size_t>(a) & static_cast<size_t>(b);
}

constexpr size_t operator&(AddrMode a, size_t b) noexcept
{
    return static_cast<size_t>(a) & b;
}

constexpr size_t operator|(size_t a, AddrMode b) noexcept
{
    return a | static_cast<size_t>(b);
}

struct VxByte final
{
public:
    u8   x;
    Byte byte;

public:
    constexpr VxByte(u16 raw) noexcept;
};

struct VxVy final
{
public:
    u8 x, y;

public:
    constexpr VxVy(u16 raw) noexcept;
};

struct VxVyN final
{
public:
    u8 x, y, n;

public:
    constexpr VxVyN(u16 raw) noexcept;
};

struct VxAddr final
{
public:
    Address addr;
    u8      x;

public:
    constexpr VxAddr(u16 raw) noexcept;
};

using Args = std::variant<Address, VxByte, VxVy, VxVyN, VxAddr, u8>;

struct OpCode final
{
public:
    Instr    instr{};
    AddrMode addressMode{};
    Args     args{};

public:
    explicit OpCode(u16 raw) noexcept;

    template<typename T>
    [[nodiscard]] constexpr T GetArgs() const noexcept
    {
        return std::get<T>(args);
    }
};

}
