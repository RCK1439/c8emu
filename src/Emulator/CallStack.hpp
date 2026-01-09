#pragma once

#include "Spec.hpp"

#include "Core/Types.hpp"

#include <array>

namespace c8emu {

class CallStack final
{
public:
    CallStack() = default;

    void PushAddr(Address addr) noexcept;
    [[nodiscard]] Address PopAddr() noexcept;

private:
    using StackBuffer = std::array<Address, C8_CALLSTACK_SIZE>;
    
    StackBuffer m_Stack{};
    std::size_t m_Ptr{};
};

}

