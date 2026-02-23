#pragma once

namespace c8emu {

template<typename T>
class Buffer final
{
public:
    constexpr Buffer() noexcept = default;
    constexpr Buffer(size_t size) noexcept :
        m_Ptr(new T[size]), m_Size(size) {}
    Buffer(const Buffer<T>&) = delete;
    Buffer(Buffer<T>&&) = delete;

    constexpr ~Buffer() noexcept { delete[] m_Ptr; }

    constexpr void Reset(size_t size) noexcept
    {
        delete[] m_Ptr;
        m_Ptr = new T[size];
        m_Size = size;
    }

    constexpr const T* GetConstPtr() const noexcept { return m_Ptr; }
    constexpr T* GetMutPtr() noexcept { return m_Ptr; }

    constexpr size_t GetSize() const noexcept { return m_Size; }

    constexpr const T& operator[](size_t idx) const noexcept { return m_Ptr[idx]; }
    constexpr T& operator[](size_t idx) noexcept { return m_Ptr[idx]; }

private:
    T*     m_Ptr{};
    size_t m_Size{};
};

}