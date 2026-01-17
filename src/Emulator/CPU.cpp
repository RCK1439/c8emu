#include "CPU.hpp"
#include "Instructions.hpp"
#include "RAM.hpp"

#include "Core/Debug.hpp"
#include "Core/Platform.hpp"
#include "Core/Random.hpp"
#include "Core/Types.hpp"

#if defined(C8_DEBUG)
#define C8_ENSURE_ADDR_MODE(addr_mode, expected)                         \
    if (((addr_mode) & (expected)) == 0 && (addr_mode) != AddrMode::NONE)\
        c8emu::Panic(c8emu::ErrorCode::INVALID_ADDRESS_MODE,             \
            "Unexpected address mode in {} executor routine: {}",        \
            __func__,                                                    \
            static_cast<c8emu::u32>(addr_mode))
#else
#define C8_ENSURE_ADDR_MODE(addr_mode, expected)
#endif

namespace c8emu {

using ExecProc = void(*)(CPU&, RAM&, const OpCode&);

void Raw(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Cls(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Ret(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Jp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Call(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Se(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Sne(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Ld(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Add(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Or(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void And(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Xor(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Sub(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Shr(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Subn(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Shl(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Rnd(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Drw(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Skp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;
void Sknp(CPU& cpu, RAM& ram, const OpCode& op) noexcept;

static constexpr ExecProc s_Executors[] = {
    Raw,
    Cls,
    Ret,
    Jp,
    Call,
    Se,
    Sne,
    Ld,
    Add,
    Or,
    And,
    Xor,
    Sub,
    Shr,
    Subn,
    Shl,
    Rnd,
    Drw,
    Skp,
    Sknp
};

void CPU::Step(RAM& ram) noexcept
{
    for (u8 i{}; i < C8_OPS_PER_CYCLE; i++)
    {
        const u16 raw = (static_cast<u16>(ram[m_PC]) << 8) | static_cast<u16>(ram[m_PC + 1]);
        m_PC += 2;

        const OpCode opcode(raw);
        s_Executors[static_cast<size_t>(opcode.instr)](*this, ram, opcode);
    }

    if (m_DT > 0)
        m_DT--;

    if (m_ST > 0)
        m_ST--;
}

void CPU::SetKey(u8 key, u8 val) noexcept
{
    m_Keypad[key] = val;
}

// --- executor implementations -----------------------------------------------

void Raw(UNUSED CPU& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::OPCODE);
    C8_LOG_ERROR("Unsupported opcode detected (0x{:X})", op.GetArgs<u16>());
}

void Cls(CPU& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::NONE);
    cpu.m_Video.fill(0x00);
}

void Ret(CPU& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::NONE);
    cpu.m_PC = cpu.m_CallStack.PopAddr();
}

void Jp(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::ADDR | AddrMode::V0_ADDR);
    
    switch (op.addressMode)
    {
        case AddrMode::ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.m_PC = addr;
        } break;
        case AddrMode::V0_ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.m_PC = cpu.m_Registers[RegisterID::V0] + addr;
        } break;
        default:
            UNREACHABLE();
    }
}

void Call(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::ADDR);
    
    const Address addr = op.GetArgs<Address>();
    cpu.m_CallStack.PushAddr(cpu.m_PC);
    cpu.m_PC = addr;
}

void Se(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            if (cpu.m_Registers[x] == byte)
                cpu.m_PC += 2;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            if (cpu.m_Registers[x] == cpu.m_Registers[y])
                cpu.m_PC += 2;
        } break;
        default:
            UNREACHABLE();
    }
}

void Sne(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            if (cpu.m_Registers[x] != byte)
                cpu.m_PC += 2;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            if (cpu.m_Registers[x] != cpu.m_Registers[y])
                cpu.m_PC += 2;
        } break;
        default:
            UNREACHABLE();
    }
}

void Ld(CPU& cpu, RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode,
        AddrMode::VX_BYTE | AddrMode::VX_VY | AddrMode::I_ADDR | AddrMode::VX_DT | AddrMode::VX_KEY | AddrMode::DT_VX |
        AddrMode::ST_VX | AddrMode::FONT_VX | AddrMode::BCD_VX | AddrMode::ADDR_I_VX | AddrMode::VX_ADDR_I);

    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            cpu.m_Registers[x] = byte;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            cpu.m_Registers[x] = cpu.m_Registers[y];
        } break;
        case AddrMode::I_ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.m_Idx = addr;
        } break;
        case AddrMode::VX_DT:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.m_Registers[x] = cpu.m_DT;
        } break;
        case AddrMode::VX_KEY:
        {
            const u8 x = op.GetArgs<u8>();

            bool found{};
            for (u8 i{}; i < C8_NUM_KEYS; i++)
            {
                if (cpu.m_Keypad[i])
                {
                    found = true;
                    cpu.m_Registers[x] = i;
                    break;
                }
            }

            if (!found)
                cpu.m_PC -= 2;
        } break;
        case AddrMode::DT_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.m_DT = cpu.m_Registers[x];
        } break;
        case AddrMode::ST_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.m_ST = cpu.m_Registers[x];
        } break;
        case AddrMode::FONT_VX:
        {
            const u8 x = op.GetArgs<u8>();
            const u8 digit = cpu.m_Registers[x];
            cpu.m_Idx = C8_ADDR_FONT + (5 * digit);
        } break;
        case AddrMode::BCD_VX:
        {
            const u8 x = op.GetArgs<u8>();

            u8 value = cpu.m_Registers[x];
            ram[cpu.m_Idx + 2] = value % 10;
            value /= 10;

            ram[cpu.m_Idx + 1] = value % 10;
            value /= 10;

            ram[cpu.m_Idx + 0] = value % 10;
        } break;
        case AddrMode::ADDR_I_VX:
        {
            const u8 x = op.GetArgs<u8>();
            for (u8 i{}; i <= x; i++)
                ram[cpu.m_Idx++] = cpu.m_Registers[i];
        } break;
        case AddrMode::VX_ADDR_I:
        {
            const u8 x = op.GetArgs<u8>();
            for (u8 i{}; i <= x; i++)
                cpu.m_Registers[i] = ram[cpu.m_Idx++];
        } break;
        default:
            UNREACHABLE();
    }
}

void Add(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY | AddrMode::I_VX);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            const u16 sum = static_cast<u16>(cpu.m_Registers[x]) + static_cast<u16>(byte);
            cpu.m_Registers[x] = static_cast<u8>(sum & 0x00FF);
            cpu.m_Registers[RegisterID::VF] = sum > 0x00FF;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            const u16 sum = static_cast<u16>(cpu.m_Registers[x]) + static_cast<u16>(cpu.m_Registers[y]);
            cpu.m_Registers[x] = static_cast<u8>(sum & 0x00FF);
            cpu.m_Registers[RegisterID::VF] = sum > 0x00FF;
        } break;
        case AddrMode::I_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.m_Idx += cpu.m_Registers[x];
        } break;
        default:
            UNREACHABLE();   
    }
}

void Or(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.m_Registers[x] |= cpu.m_Registers[y];
    cpu.m_Registers[RegisterID::VF] = 0;
}

void And(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.m_Registers[x] &= cpu.m_Registers[y];
    cpu.m_Registers[RegisterID::VF] = 0;
}

void Xor(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.m_Registers[x] ^= cpu.m_Registers[y];
    cpu.m_Registers[RegisterID::VF] = 0;
}

void Sub(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u16 diff = static_cast<u16>(cpu.m_Registers[x]) - static_cast<u16>(cpu.m_Registers[y]);
    cpu.m_Registers[x] = static_cast<u8>(diff & 0x00FF);
    cpu.m_Registers[RegisterID::VF] = diff <= 0x00FF;
}

void Shr(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u8 bit = cpu.m_Registers[y] & 0x01;
    cpu.m_Registers[x] = cpu.m_Registers[y] >> 1;
    cpu.m_Registers[RegisterID::VF] = bit > 0;
}

void Subn(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u16 diff = static_cast<u16>(cpu.m_Registers[y]) - static_cast<u16>(cpu.m_Registers[x]);
    cpu.m_Registers[x] = static_cast<u8>(diff & 0x00FF);
    cpu.m_Registers[RegisterID::VF] = diff <= 0x00FF;
}

void Shl(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u8 bit = cpu.m_Registers[y] & 0x80;
    cpu.m_Registers[x] = cpu.m_Registers[y] << 1;
    cpu.m_Registers[RegisterID::VF] = bit > 0;
}

void Rnd(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE);
    const auto [x, byte] = op.GetArgs<VxByte>();
    cpu.m_Registers[x] = static_cast<u8>(Random::GetValue<i32>(0, 255)) & byte;
}

void Drw(CPU& cpu, RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY_N);

    const auto [x, y, n] = op.GetArgs<VxVyN>();
    
    const u8 height = n;
    const u8 x0 = cpu.m_Registers[x] % C8_SCREEN_BUFFER_WIDTH<u8>;
    const u8 y0 = cpu.m_Registers[y] % C8_SCREEN_BUFFER_HEIGHT<u8>;

    cpu.m_Registers[RegisterID::VF] = 0;
    for (u8 vy{}; vy < height; vy++)
    {
        const u16 y1 = static_cast<u16>(y0 + vy);
        if (y1 >= C8_SCREEN_BUFFER_HEIGHT<u16>)
            continue;

        const u8 sprite = ram[cpu.m_Idx + vy];
        for (u8 vx{}; vx < 8; vx++)
        {
            const u16 x1 = static_cast<u16>(x0 + vx);
            if (x1 >= C8_SCREEN_BUFFER_WIDTH<u16>)
                continue;

            const u8 spritePx = sprite & (0x80 >> vx);
            if (spritePx > 0)
            {
                const u16 idx = x1 + y1 * C8_SCREEN_BUFFER_WIDTH<u16>;
                if (cpu.m_Video[idx] == 0xFF)
                    cpu.m_Registers[RegisterID::VF] = 1;

                cpu.m_Video[idx] ^= 0xFF;
            }
        }
    }
}

void Skp(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX);
    const u8 x = op.GetArgs<u8>();
    const u8 key = cpu.m_Registers[x];
    if (cpu.m_Keypad[key])
        cpu.m_PC += 2;
}

void Sknp(CPU& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX);
    const u8 x = op.GetArgs<u8>();
    const u8 key = cpu.m_Registers[x];
    if (!cpu.m_Keypad[key])
        cpu.m_PC += 2;
}

}
