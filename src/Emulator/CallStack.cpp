#include "CallStack.hpp"

#include "Core/Debug.hpp"

namespace c8emu {

void CallStack::PushAddr(u16 addr) noexcept
{
    C8_ASSERT(m_Ptr < C8_CALLSTACK_SIZE, "Cannot push; call stack is full");
    m_Stack[static_cast<std::size_t>(m_Ptr++)] = addr;
}

u16 CallStack::PopAddr() noexcept
{
    C8_ASSERT(m_Ptr > 0, "Cannot pop; call stack is empty");
    return m_Stack.at(static_cast<std::size_t>(--m_Ptr));
}

}
