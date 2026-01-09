#include "CallStack.hpp"

#include "Core/Debug.hpp"

namespace c8emu {

void CallStack::PushAddr(Address addr) noexcept
{
    C8_ASSERT(m_Ptr < C8_CALLSTACK_SIZE, "Cannot push; call stack is full");
    m_Stack[m_Ptr++] = addr;
}

[[nodiscard]] Address CallStack::PopAddr() noexcept
{
    C8_ASSERT(m_Ptr > 0, "Cannot pop; call stack is empty");
    return m_Stack.at(--m_Ptr);
}

}
