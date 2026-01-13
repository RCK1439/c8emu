#pragma once

#include <random>

namespace c8emu {

class Random final
{
public:
    template<typename T>
        requires std::is_integral_v<T>
    static T GetValue(T min, T max) noexcept
    {
        static std::mt19937 generator;
        static std::uniform_int_distribution<T> distribution(min, max);

        return distribution(generator);
    }
};

}