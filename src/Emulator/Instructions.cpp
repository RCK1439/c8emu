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

using DecodeProc = OpCode (*)(u16);

static OpCode Decode0(u16 raw) noexcept;
static OpCode Decode1(u16 raw) noexcept;
static OpCode Decode2(u16 raw) noexcept;
static OpCode Decode3(u16 raw) noexcept;
static OpCode Decode4(u16 raw) noexcept;
static OpCode Decode5(u16 raw) noexcept;
static OpCode Decode6(u16 raw) noexcept;
static OpCode Decode7(u16 raw) noexcept;
static OpCode Decode8(u16 raw) noexcept;
static OpCode Decode9(u16 raw) noexcept;
static OpCode DecodeA(u16 raw) noexcept;
static OpCode DecodeB(u16 raw) noexcept;
static OpCode DecodeC(u16 raw) noexcept;
static OpCode DecodeD(u16 raw) noexcept;
static OpCode DecodeE(u16 raw) noexcept;
static OpCode DecodeF(u16 raw) noexcept;

static constexpr DecodeProc s_Decoders[] = {
    Decode0, Decode1, Decode2, Decode3,
    Decode4, Decode5, Decode6, Decode7,
    Decode8, Decode9, DecodeA, DecodeB,
    DecodeC, DecodeD, DecodeE, DecodeF
};

OpCode DecodeOpCode(u16 raw) noexcept
{
    const size_t idx = (size_t)(Instr(raw));
    return s_Decoders[idx](raw);
}

static OpCode Decode0(u16 raw) noexcept
{
    OpCode code = {};

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

    return code;
}

static OpCode Decode1(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::JP,
        .addressMode = AddrMode::ADDR,
        .args = NNN(raw),
    };

    return code;
}

static OpCode Decode2(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::CALL,
        .addressMode = AddrMode::ADDR,
        .args = NNN(raw),
    };

    return code;
}

static OpCode Decode3(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::SE,
        .addressMode = AddrMode::VX_BYTE,
        .args = VxByte(raw),
    };

    return code;
}

static OpCode Decode4(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::SNE,
        .addressMode = AddrMode::VX_BYTE,
        .args = VxByte(raw),
    };

    return code;
}

static OpCode Decode5(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::SE,
        .addressMode = AddrMode::VX_VY,
        .args = VxVy(raw),
    };

    return code;
}

static OpCode Decode6(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::LD,
        .addressMode = AddrMode::VX_BYTE,
        .args = VxByte(raw),
    };

    return code;
}

static OpCode Decode7(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::ADD,
        .addressMode = AddrMode::VX_BYTE,
        .args = VxByte(raw),
    };

    return code;
}

static OpCode Decode8(u16 raw) noexcept
{
    OpCode code = {
        .instr = Instr::RAW,
        .addressMode = AddrMode::VX_VY,
        .args = VxVy(raw),
    };

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

    return code;
}

static OpCode Decode9(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::SNE,
        .addressMode = AddrMode::VX_VY,
        .args = VxVy(raw),
    };

    return code;
}

static OpCode DecodeA(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::LD,
        .addressMode = AddrMode::I_ADDR,
        .args = NNN(raw),
    };
    
    return code;
}

static OpCode DecodeB(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::JP, 
        .addressMode = AddrMode::V0_ADDR,
        .args = NNN(raw),
    };

    return code;
}

static OpCode DecodeC(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::RND,
        .addressMode = AddrMode::VX_BYTE,
        .args = VxByte(raw),
    };
    
    return code;
}

static OpCode DecodeD(u16 raw) noexcept
{
    const OpCode code = {
        .instr = Instr::DRW,
        .addressMode = AddrMode::VX_VY_N,
        .args = VxVyN(raw),
    };
    
    return code;
}

static OpCode DecodeE(u16 raw) noexcept
{
    OpCode code = {};

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

    return code;
}

static OpCode DecodeF(u16 raw) noexcept
{
    OpCode code = {};

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

    return code;
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
