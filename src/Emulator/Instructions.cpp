#include "Instructions.hpp"

namespace c8emu {

inline static constexpr u8 Instr(u16 raw) noexcept
{
    return static_cast<u8>((raw & 0xF000) >> 12);
}

inline static constexpr u8 X(u16 raw) noexcept
{
    return static_cast<u8>((raw & 0x0F00) >> 8);
}

inline static constexpr u8 Y(u16 raw) noexcept
{
    return static_cast<u8>((raw & 0x00F0) >> 4);
}

inline static constexpr u8 N(u16 raw) noexcept
{
    return static_cast<u8>(raw & 0x000F);
}

inline static constexpr u8 KK(u16 raw) noexcept
{
    return static_cast<u8>(raw & 0x00FF);
}

inline static constexpr u16 NNN(u16 raw) noexcept
{
    return raw & 0x0FFF;
}

using DecodeProc = void (*)(OpCode&, u16);

static void Decode0(OpCode& code, u16 raw) noexcept;
static void Decode1(OpCode& code, u16 raw) noexcept;
static void Decode2(OpCode& code, u16 raw) noexcept;
static void Decode3(OpCode& code, u16 raw) noexcept;
static void Decode4(OpCode& code, u16 raw) noexcept;
static void Decode5(OpCode& code, u16 raw) noexcept;
static void Decode6(OpCode& code, u16 raw) noexcept;
static void Decode7(OpCode& code, u16 raw) noexcept;
static void Decode8(OpCode& code, u16 raw) noexcept;
static void Decode9(OpCode& code, u16 raw) noexcept;
static void DecodeA(OpCode& code, u16 raw) noexcept;
static void DecodeB(OpCode& code, u16 raw) noexcept;
static void DecodeC(OpCode& code, u16 raw) noexcept;
static void DecodeD(OpCode& code, u16 raw) noexcept;
static void DecodeE(OpCode& code, u16 raw) noexcept;
static void DecodeF(OpCode& code, u16 raw) noexcept;

static constexpr DecodeProc s_Decoders[] = {
    Decode0, Decode1, Decode2, Decode3,
    Decode4, Decode5, Decode6, Decode7,
    Decode8, Decode9, DecodeA, DecodeB,
    DecodeC, DecodeD, DecodeE, DecodeF
};

OpCode::OpCode(u16 raw) noexcept
{
    const size_t idx = (size_t)(Instr(raw));
    s_Decoders[idx](*this, raw);
}

static void Decode0(OpCode& code, u16 raw) noexcept
{
    switch (raw & 0x00FF)
    {
        case 0xE0:
        {
            code.instr = Instr::CLS;
            code.addressMode = AddrMode::NONE;
        } break;
        case 0xEE:
        {
            code.instr = Instr::RET;
            code.addressMode = AddrMode::NONE;
        } break;
        default:
        {
            code.instr = Instr::RAW;
            code.addressMode = AddrMode::OPCODE;
            code.args = raw;
        } break;
    }
}

static void Decode1(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::JP;
    code.addressMode = AddrMode::ADDR;
    code.args = NNN(raw);
}

static void Decode2(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::CALL;
    code.addressMode = AddrMode::ADDR;
    code.args = NNN(raw);
}

static void Decode3(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::SE;
    code.addressMode = AddrMode::VX_BYTE;
    code.args = VxByte(raw);
}

static void Decode4(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::SNE;
    code.addressMode = AddrMode::VX_BYTE;
    code.args = VxByte(raw);
}

static void Decode5(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::SE;
    code.addressMode = AddrMode::VX_VY;
    code.args = VxVy(raw);
}

static void Decode6(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::LD;
    code.addressMode = AddrMode::VX_BYTE;
    code.args = VxByte(raw);
}

static void Decode7(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::ADD;
    code.addressMode = AddrMode::VX_BYTE;
    code.args = VxByte(raw);
}

static void Decode8(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::RAW;
    code.addressMode = AddrMode::VX_VY;
    code.args = VxVy(raw);

    switch (raw & 0x000F)
    {
        case 0x0: code.instr = Instr::LD; break;
        case 0x1: code.instr = Instr::OR; break;
        case 0x2: code.instr = Instr::AND; break;
        case 0x3: code.instr = Instr::XOR; break;
        case 0x4: code.instr = Instr::ADD; break;
        case 0x5: code.instr = Instr::SUB; break;
        case 0x6: code.instr = Instr::SHR; break;
        case 0x7: code.instr = Instr::SUBN; break;
        case 0xE: code.instr = Instr::SHL; break;
        default:
        {
            code.instr = Instr::RAW;
            code.addressMode = AddrMode::OPCODE;
            code.args = raw;
        } break;
    }
}

static void Decode9(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::SNE;
    code.addressMode = AddrMode::VX_VY;
    code.args = VxVy(raw);
}

static void DecodeA(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::LD;
    code.addressMode = AddrMode::I_ADDR;
    code.args = NNN(raw);
}

static void DecodeB(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::JP;
    code.addressMode = AddrMode::V0_ADDR;
    code.args = NNN(raw);
}

static void DecodeC(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::RND;
    code.addressMode = AddrMode::VX_BYTE;
    code.args = VxByte(raw);
}

static void DecodeD(OpCode& code, u16 raw) noexcept
{
    code.instr = Instr::DRW;
    code.addressMode = AddrMode::VX_VY_N;
    code.args = VxVyN(raw);
}

static void DecodeE(OpCode& code, u16 raw) noexcept
{
    switch (raw & 0x00FF)
    {
        case 0x9E:
        {
            code.instr = Instr::SKP;
            code.addressMode = AddrMode::VX;
            code.args = X(raw);
        } break;
        case 0xA1:
        {
            code.instr = Instr::SKNP;
            code.addressMode = AddrMode::VX;
            code.args = X(raw);
        } break;
        default:
        {
            code.instr = Instr::RAW;
            code.addressMode = AddrMode::OPCODE;
            code.args = raw;
        } break;
    }
}

static void DecodeF(OpCode& code, u16 raw) noexcept
{
    switch (raw & 0x00FF)
    {
        case 0x07:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::VX_DT;
            code.args = X(raw);
        } break;
        case 0x0A:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::VX_KEY;
            code.args = X(raw);
        } break;
        case 0x15:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::DT_VX;
            code.args = X(raw);
        } break;
        case 0x18:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::ST_VX;
            code.args = X(raw);
        } break;
        case 0x1E:
        {
            code.instr = Instr::ADD;
            code.addressMode = AddrMode::I_VX;
            code.args = X(raw);
        } break;
        case 0x29:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::FONT_VX;
            code.args = X(raw);
        } break;
        case 0x33:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::BCD_VX;
            code.args = X(raw);
        } break;
        case 0x55:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::ADDR_I_VX;
            code.args = X(raw);
        } break;
        case 0x65:
        {
            code.instr = Instr::LD;
            code.addressMode = AddrMode::VX_ADDR_I;
            code.args = X(raw);
        } break;
        default:
        {
            code.instr = Instr::RAW;
            code.addressMode = AddrMode::OPCODE;
            code.args = raw;
        } break;
    }
}

constexpr VxByte::VxByte(u16 raw) noexcept :
    x(X(raw)), byte(KK(raw)) {}

constexpr VxVy::VxVy(u16 raw) noexcept :
    x(X(raw)), y(Y(raw)) {}

constexpr VxVyN::VxVyN(u16 raw) noexcept :
    x(X(raw)), y(Y(raw)), n(N(raw)) {}

constexpr VxAddr::VxAddr(u16 raw) noexcept :
    addr(NNN(raw)), x(X(raw)) {}

}
