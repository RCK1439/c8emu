#pragma once

#include <chrono>

namespace c8emu {

class Time
{
public:
    static float Now() noexcept
    {
        const std::chrono::duration<float> now = std::chrono::high_resolution_clock::now()
            .time_since_epoch();
        
        return now.count();
    }
};

}