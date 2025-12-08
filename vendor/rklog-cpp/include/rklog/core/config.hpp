#pragma once

#include "time.hpp"

#include <cstdint>
#include <optional>
#include <string_view>

namespace rklog {

// --- escape codes -----------------------------------------------------------

#define RKLOG_ESC_CODE_START "\033["
#define RKLOG_ESC_CODE_END   "m"

#define RKLOG_ESC_CODE_RESET RKLOG_ESC_CODE_START "0" RKLOG_ESC_CODE_END

#define RKLOG_TRUE_COLOR_MODE "2;"
#define RKLOG_256_COLOR_PALETTE_MODE "5;"

#define RKLOG_FG_COLOR_MODE "38;"
#define RKLOG_BG_COLOR_MODE "48;"

// --- format specifiers ------------------------------------------------------

#define RKLOG_FG_COLOR_FMT\
    RKLOG_FG_COLOR_MODE RKLOG_TRUE_COLOR_MODE "{};{};{}"
#define RKLOG_BG_COLOR_FMT\
    RKLOG_BG_COLOR_MODE RKLOG_TRUE_COLOR_MODE "{};{};{}"

#define RKLOG_FMT_COLOR_PRELUDE\
    RKLOG_ESC_CODE_START RKLOG_FG_COLOR_FMT ";" RKLOG_BG_COLOR_FMT RKLOG_ESC_CODE_END

#define RKLOG_FMT_COLOR_PRELUDE_NO_BG\
    RKLOG_ESC_CODE_START RKLOG_FG_COLOR_FMT RKLOG_ESC_CODE_END

#define RKLOG_FMT_LABEL "[{}]:[{}]:[{}]: "

// --- type definitions -------------------------------------------------------

/**
 * Class representing color in an RGB format
 */
struct Color final
{
public:
    const uint8_t r = 0xFF; // Red channel
    const uint8_t g = 0xFF; // Green channel
    const uint8_t b = 0xFF; // Blue channel

public:
    /**
     * Creates a new instance of `Color` with the given RGB channel values.
     *
     * @param r
     *      The red channel value
     * @param g
     *      The green channel value
     * @param b
     *      The blue channel value
     */
    constexpr Color(uint8_t r, uint8_t g, uint8_t b) noexcept :
        r(r), g(g), b(b) {}
};

/**
 * Class representing a configuration for a specific log-level
 */
class LogConfig final
{
public:
    LogConfig() = delete;

    /**
     * Creates an instance of `LogConfig` with a foreground color
     *
     * @param tag
     *      A tag representing the log level
     * @param foreground
     *      The foreground color of the log text
     */
    constexpr explicit LogConfig(std::string_view tag, Color foreground) noexcept :
        m_Tag(tag), m_Foreground(foreground) {}

    /**
     * Creates an instance of `LogConfig` with a foreground and background
     * color
     *
     * @param tag
     *      A tag representing the log level
     * @param foreground
     *      The foreground color of the log text
     * @param background
     *      The background color of the log text
     */
    constexpr explicit LogConfig(std::string_view tag, Color foreground, Color background) noexcept :
        m_Tag(tag), m_Foreground(foreground), m_Background(background) {}

    /**
     * Generates the log label in format of `[title]:[tag]:[hrs:mins:secs]: `
     *
     * @return
     *      The label of the log message
     */
    std::string GenerateLabel(std::string_view title, const TimeStamp& ts) const
    {
        return std::format(RKLOG_FMT_LABEL, title, m_Tag, ts);
    }

    /**
     * Generates the ASCII color prelude for logging colored text
     *
     * @return
     *      The ASCII color prelude
     */
    std::string GenerateColorPrelude() const
    {
        if (m_Background.has_value())
        {
            const Color background = m_Background.value();
            return std::format(RKLOG_FMT_COLOR_PRELUDE, m_Foreground.r, m_Foreground.g, m_Foreground.b, background.r, background.g, background.b);
        }
        
        return std::format(RKLOG_FMT_COLOR_PRELUDE_NO_BG, m_Foreground.r, m_Foreground.g, m_Foreground.b);
    }

private:
    const std::string_view     m_Tag;                       // Tag title for the severity level
    const Color                m_Foreground;                // Background color
    const std::optional<Color> m_Background = std::nullopt; // Foreground color
};

}