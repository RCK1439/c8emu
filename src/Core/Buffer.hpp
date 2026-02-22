#pragma once

namespace c8emu {

template<typename T>
class Buffer
{
public:
    constexpr Buffer() = default;
    constexpr Buffer(size_t size) :
        m_Ptr(new T[size]), m_Size(size) {}
    Buffer(const Buffer<T>&) = delete;
    Buffer(Buffer<T>&&) = delete;

    constexpr ~Buffer() { delete[] m_Ptr; }

    constexpr void Reset(size_t size)
    {
        delete[] m_Ptr;
        m_Ptr = new T[size];
        m_Size = size;
    }

    constexpr const T* Get() const { return m_Ptr; }
    constexpr T* Get() { return m_Ptr; }

    constexpr size_t Size() const { return m_Size; }

    constexpr const T& operator[](size_t idx) const { return m_Ptr[idx]; }
    constexpr T& operator[](size_t idx) { return m_Ptr[idx]; }

private:
    T*     m_Ptr{};
    size_t m_Size{};
};

}