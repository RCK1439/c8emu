#pragma once

#include "Core/Types.hpp"

#include <limits>
#include <random>

namespace c8emu {

class Random final
{
public:
    template<typename T>
        requires std::is_integral_v<T>
    static constexpr T GetValue() noexcept
    {
        constexpr T MIN = std::numeric_limits<T>::min();
        constexpr T MAX = std::numeric_limits<T>::max();

        static std::mt19937 generator;
        static std::uniform_int_distribution<i64> distribution(MIN, MAX);

        return static_cast<T>(distribution(generator));
    }
};

}