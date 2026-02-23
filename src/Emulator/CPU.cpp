#include "CPU.hpp"
#include "Instructions.hpp"
#include "RAM.hpp"

#include "Core/Debug.hpp"
#include "Core/Platform.hpp"
#include "Core/Random.hpp"

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

static void Raw(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Cls(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Ret(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Jp(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Call(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Se(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Sne(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Ld(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Add(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Or(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void And(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Xor(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Sub(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Shr(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Subn(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Shl(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Rnd(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Drw(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Skp(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;
static void Sknp(CPUData& cpu, RAM& ram, const OpCode& op) noexcept;

using ExecProc = void(*)(CPUData&, RAM&, const OpCode&) noexcept;

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
        const u16 raw = (static_cast<u16>(ram[m_Data.PC]) << 8) | static_cast<u16>(ram[m_Data.PC + 1]);
        m_Data.PC += 2;

        const OpCode opcode(raw);
        s_Executors[static_cast<size_t>(opcode.instr)](m_Data, ram, opcode);
    }

    if (m_Data.DT > 0)
        m_Data.DT--;

    if (m_Data.ST > 0)
        m_Data.ST--;
}

void CPU::SetKey(u8 key, u8 val) noexcept
{
    m_Data.Keypad[key] = val;
}

// --- executor implementations -----------------------------------------------

static void Raw(UNUSED CPUData& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::OPCODE);
    C8_LOG_ERROR("Unsupported opcode detected (0x{:X})", op.GetArgs<u16>());
}

static void Cls(CPUData& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::NONE);
    cpu.Video.fill(0x00);
}

static void Ret(CPUData& cpu, UNUSED RAM& ram, UNUSED const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::NONE);
    cpu.PC = cpu.CallStack.PopAddr();
}

static void Jp(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::ADDR | AddrMode::V0_ADDR);
    
    switch (op.addressMode)
    {
        case AddrMode::ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.PC = addr;
        } break;
        case AddrMode::V0_ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.PC = cpu.Registers[RegisterID::V0] + addr;
        } break;
        default:
            UNREACHABLE();
    }
}

static void Call(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::ADDR);
    
    const Address addr = op.GetArgs<Address>();
    cpu.CallStack.PushAddr(cpu.PC);
    cpu.PC = addr;
}

static void Se(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            if (cpu.Registers[x] == byte)
                cpu.PC += 2;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            if (cpu.Registers[x] == cpu.Registers[y])
                cpu.PC += 2;
        } break;
        default:
            UNREACHABLE();
    }
}

static void Sne(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            if (cpu.Registers[x] != byte)
                cpu.PC += 2;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            if (cpu.Registers[x] != cpu.Registers[y])
                cpu.PC += 2;
        } break;
        default:
            UNREACHABLE();
    }
}

static void Ld(CPUData& cpu, RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode,
        AddrMode::VX_BYTE | AddrMode::VX_VY | AddrMode::I_ADDR | AddrMode::VX_DT | AddrMode::VX_KEY | AddrMode::DT_VX |
        AddrMode::ST_VX | AddrMode::FONT_VX | AddrMode::BCD_VX | AddrMode::ADDR_I_VX | AddrMode::VX_ADDR_I);

    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            cpu.Registers[x] = byte;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            cpu.Registers[x] = cpu.Registers[y];
        } break;
        case AddrMode::I_ADDR:
        {
            const Address addr = op.GetArgs<Address>();
            cpu.Idx = addr;
        } break;
        case AddrMode::VX_DT:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.Registers[x] = cpu.DT;
        } break;
        case AddrMode::VX_KEY:
        {
            const u8 x = op.GetArgs<u8>();

            bool found{};
            for (u8 i{}; i < C8_NUM_KEYS; i++)
            {
                if (cpu.Keypad[i])
                {
                    found = true;
                    cpu.Registers[x] = i;
                    break;
                }
            }

            if (!found)
                cpu.PC -= 2;
        } break;
        case AddrMode::DT_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.DT = cpu.Registers[x];
        } break;
        case AddrMode::ST_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.ST = cpu.Registers[x];
        } break;
        case AddrMode::FONT_VX:
        {
            const u8 x = op.GetArgs<u8>();
            const u8 digit = cpu.Registers[x];
            cpu.Idx = C8_ADDR_FONT + (5 * digit);
        } break;
        case AddrMode::BCD_VX:
        {
            const u8 x = op.GetArgs<u8>();

            u8 value = cpu.Registers[x];
            ram[cpu.Idx + 2] = value % 10;
            value /= 10;

            ram[cpu.Idx + 1] = value % 10;
            value /= 10;

            ram[cpu.Idx + 0] = value % 10;
        } break;
        case AddrMode::ADDR_I_VX:
        {
            const u8 x = op.GetArgs<u8>();
            for (u8 i{}; i <= x; i++)
                ram[cpu.Idx++] = cpu.Registers[i];
        } break;
        case AddrMode::VX_ADDR_I:
        {
            const u8 x = op.GetArgs<u8>();
            for (u8 i{}; i <= x; i++)
                cpu.Registers[i] = ram[cpu.Idx++];
        } break;
        default:
            UNREACHABLE();
    }
}

static void Add(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE | AddrMode::VX_VY | AddrMode::I_VX);
    switch (op.addressMode)
    {
        case AddrMode::VX_BYTE:
        {
            const auto [x, byte] = op.GetArgs<VxByte>();
            const u16 sum = static_cast<u16>(cpu.Registers[x]) + static_cast<u16>(byte);
            cpu.Registers[x] = static_cast<u8>(sum & 0x00FF);
            cpu.Registers[RegisterID::VF] = sum > 0x00FF;
        } break;
        case AddrMode::VX_VY:
        {
            const auto [x, y] = op.GetArgs<VxVy>();
            const u16 sum = static_cast<u16>(cpu.Registers[x]) + static_cast<u16>(cpu.Registers[y]);
            cpu.Registers[x] = static_cast<u8>(sum & 0x00FF);
            cpu.Registers[RegisterID::VF] = sum > 0x00FF;
        } break;
        case AddrMode::I_VX:
        {
            const u8 x = op.GetArgs<u8>();
            cpu.Idx += cpu.Registers[x];
        } break;
        default:
            UNREACHABLE();   
    }
}

static void Or(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.Registers[x] |= cpu.Registers[y];
    cpu.Registers[RegisterID::VF] = 0;
}

static void And(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.Registers[x] &= cpu.Registers[y];
    cpu.Registers[RegisterID::VF] = 0;
}

static void Xor(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    cpu.Registers[x] ^= cpu.Registers[y];
    cpu.Registers[RegisterID::VF] = 0;
}

static void Sub(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u16 diff = static_cast<u16>(cpu.Registers[x]) - static_cast<u16>(cpu.Registers[y]);
    cpu.Registers[x] = static_cast<u8>(diff & 0x00FF);
    cpu.Registers[RegisterID::VF] = diff <= 0x00FF;
}

static void Shr(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u8 bit = cpu.Registers[y] & 0x01;
    cpu.Registers[x] = cpu.Registers[y] >> 1;
    cpu.Registers[RegisterID::VF] = bit > 0;
}

static void Subn(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u16 diff = static_cast<u16>(cpu.Registers[y]) - static_cast<u16>(cpu.Registers[x]);
    cpu.Registers[x] = static_cast<u8>(diff & 0x00FF);
    cpu.Registers[RegisterID::VF] = diff <= 0x00FF;
}

static void Shl(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY);
    const auto [x, y] = op.GetArgs<VxVy>();
    const u8 bit = cpu.Registers[y] & 0x80;
    cpu.Registers[x] = cpu.Registers[y] << 1;
    cpu.Registers[RegisterID::VF] = bit > 0;
}

static void Rnd(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_BYTE);
    const auto [x, byte] = op.GetArgs<VxByte>();
    cpu.Registers[x] = Random::GetValue<u8>() & byte;
}

static void Drw(CPUData& cpu, RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX_VY_N);

    const auto [x, y, n] = op.GetArgs<VxVyN>();
    
    const u8 height = n;
    const u8 x0 = cpu.Registers[x] % C8_SCREEN_BUFFER_WIDTH<u8>;
    const u8 y0 = cpu.Registers[y] % C8_SCREEN_BUFFER_HEIGHT<u8>;

    cpu.Registers[RegisterID::VF] = 0;
    for (u8 vy{}; vy < height; vy++)
    {
        const u16 y1 = static_cast<u16>(y0 + vy);
        if (y1 >= C8_SCREEN_BUFFER_HEIGHT<u16>)
            continue;

        const u8 sprite = ram[cpu.Idx + vy];
        for (u8 vx{}; vx < 8; vx++)
        {
            const u16 x1 = static_cast<u16>(x0 + vx);
            if (x1 >= C8_SCREEN_BUFFER_WIDTH<u16>)
                continue;

            const u8 spritePx = sprite & (0x80 >> vx);
            if (spritePx > 0)
            {
                const u16 idx = x1 + y1 * C8_SCREEN_BUFFER_WIDTH<u16>;
                if (cpu.Video[idx] == 0xFF)
                    cpu.Registers[RegisterID::VF] = 1;

                cpu.Video[idx] ^= 0xFF;
            }
        }
    }
}

static void Skp(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX);
    const u8 x = op.GetArgs<u8>();
    const u8 key = cpu.Registers[x];
    if (cpu.Keypad[key])
        cpu.PC += 2;
}

static void Sknp(CPUData& cpu, UNUSED RAM& ram, const OpCode& op) noexcept
{
    C8_ENSURE_ADDR_MODE(op.addressMode, AddrMode::VX);
    const u8 x = op.GetArgs<u8>();
    const u8 key = cpu.Registers[x];
    if (!cpu.Keypad[key])
        cpu.PC += 2;
}

}
