#pragma once

#include "Spec.hpp"

#include "Core/Types.hpp"

#include <array>

namespace c8emu {

class CallStack final
{
public:
    CallStack() = default;

    void PushAddr(u16 addr) noexcept;
    u16 PopAddr() noexcept;

private:
    using StackBuffer = std::array<u16, C8_CALLSTACK_SIZE>;
    
    StackBuffer m_Stack = { 0 };
    u8          m_Ptr   = 0;
};

}

