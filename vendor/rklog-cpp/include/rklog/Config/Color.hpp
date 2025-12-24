#pragma once

#include <cstdint>

namespace rklog {

/**
 * Struct containing color data for color logging using true color
 */
struct Color final
{
public:
    uint8_t r = 0x00; // The red channel
    uint8_t g = 0x00; // The green channel
    uint8_t b = 0x00; // The blue channel

public:
    constexpr Color() = default;

    /**
     * Creates a new instance of the color struct with defined values for the
     * color channels
     *
     * @param[in] r
     *      The value of the red channel
     * @param[in] g
     *      The value of the green channel
     * @param[in] b
     *      The value of the blue channel
     */
    constexpr Color(uint8_t r, uint8_t g, uint8_t b) noexcept :
        r(r), g(g), b(b) {}
};

}

namespace rklog::defaults {

constexpr Color COLOR_RED       = Color(255, 0, 0);
constexpr Color COLOR_GREEN     = Color(0, 255, 0);
constexpr Color COLOR_BLUE      = Color(0, 0, 255);
constexpr Color COLOR_YELLOW    = Color(255, 255, 0);
constexpr Color COLOR_MAGENTA   = Color(255, 0, 255);
constexpr Color COLOR_LIGHTBLUE = Color(0, 255, 255);
constexpr Color COLOR_WHITE     = Color(255, 255, 255);
constexpr Color COLOR_BLACK     = Color(0, 0, 0);

}