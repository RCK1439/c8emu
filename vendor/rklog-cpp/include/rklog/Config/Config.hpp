#pragma once

#include "Color.hpp"
#include "Level.hpp"

#include "../Core/Platform.hpp"

#include <optional>
#include <string_view>

namespace rklog {

/**
 * Class containing configuration data for a log level
 */
class LogConfig final
{
public:
    /**
     * Gets the log level for which this configuration is configured for
     *
     * @return
     *      The log level for this log configuration
     */
    constexpr LogLevel GetLevel() const noexcept
    {
        return m_Level;
    }

    /**
     * Gets the tag for the log level
     *
     * @return
     *      The tag for the log level
     */
    constexpr std::string_view GetTag() const noexcept
    {
        if (m_Tag.has_value())
        {
            return m_Tag.value();
        }

        switch (m_Level)
        {
            case LogLevel::LOG_DEBUG:
                return "DEBUG";
            case LogLevel::LOG_INFO:
                return "INFO";
            case LogLevel::LOG_WARNING:
                return "WARNING";
            case LogLevel::LOG_ERROR:
                return "ERROR";
            case LogLevel::LOG_FATAL:
                return "FATAL";
        }

        RKLOG_UNREACHABLE();
    }

    /**
     * Gets the foreground color of the log level
     *
     * @return
     *      The foreground color data
     */
    constexpr std::optional<Color> GetForegroundColor() const noexcept
    {
        return m_Foreground;
    }

    /**
     * Gets the background color of the log level
     *
     * @return
     *      The background color data
     */
    constexpr std::optional<Color> GetBackgroundColor() const noexcept
    {
        return m_Background;
    }

private:
    /**
     * Creates a new instance of the log configuration for the given log level
     *
     * @param[in] level
     *      The level for which this instance is the configuration for
     */
    constexpr LogConfig(LogLevel level) noexcept :
        m_Level(level) {}

private:
    /// The log level
    LogLevel m_Level;
    /// The foreground color data
    std::optional<Color> m_Foreground = std::nullopt;
    /// The background color data
    std::optional<Color> m_Background = std::nullopt;
    /// The tag for the log level
    std::optional<std::string_view> m_Tag = std::nullopt;

    friend class LogConfigBuilder;
};

/**
 * Class used to build configurations for log levels
 */
class LogConfigBuilder final
{
public:
    LogConfigBuilder(const LogConfigBuilder&) = delete;
    LogConfigBuilder(LogConfigBuilder&&) = delete;

    /**
     * Sets the tag for the log configuration
     *
     * @param[in] tag
     *      The tag of the log configuration
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogConfigBuilder& SetTag(std::string_view tag) noexcept
    {
        m_Config.m_Tag = tag;
        return *this;
    }

    /**
     * Sets the foreground color of the log configuration
     *
     * @param[in] color
     *      The foreground color
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogConfigBuilder& SetForeground(Color color) noexcept
    {
        m_Config.m_Foreground = color;
        return *this;
    }

    /**
     * Sets the foreground color of the log configuration with color channel
     * values
     *
     * @param[in] r
     *      The red channel value
     * @param[in] g
     *      The green channel value
     * @param[in] b
     *      The blue channel value
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogConfigBuilder& SetForeground(uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        m_Config.m_Foreground = Color(r, g, b);
        return *this;
    }

    /**
     * Sets the background color of the log configuration
     *
     * @param[in] color
     *      The foreground color
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogConfigBuilder& SetBackground(Color color) noexcept
    {
        m_Config.m_Background = color;
        return *this;
    }

    /**
     * Sets the background color of the log configuration with color channel
     * values
     *
     * @param[in] r
     *      The red channel value
     * @param[in] g
     *      The green channel value
     * @param[in] b
     *      The blue channel value
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogConfigBuilder& SetBackground(uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        m_Config.m_Background = Color(r, g, b);
        return *this;
    }

    /**
     * Finalizes the build for the log configuration
     *
     * @return
     *      The final log configuration
     */
    [[nodiscard]] constexpr LogConfig&& Build() noexcept
    {
        return std::move(m_Config);
    }

private:
    /**
     * Creates a new instance of the configuration builder with a defined log
     * level
     *
     * @param[in] level
     *      The log level that this builder will be creating the configuration
     *      for
     */
    constexpr LogConfigBuilder(LogLevel level) noexcept :
        m_Config(level) {}

private:
    /// The configuration that this builder is building
    LogConfig m_Config;

    friend constexpr LogConfigBuilder InitBuildConfig(LogLevel) noexcept;
};

/**
 * Initializes the building process for the configuration of a log level
 *
 * @param[in] level
 *      The log level to create a configuration for
 *
 * @return
 *      An instance of the config builder 
 */
[[nodiscard]] constexpr LogConfigBuilder InitBuildConfig(LogLevel level) noexcept
{
    return LogConfigBuilder(level);
}

}

namespace rklog::defaults {

constexpr auto DEBUG_CFG = InitBuildConfig(LogLevel::LOG_DEBUG)
    .SetForeground(COLOR_BLUE)
    .Build();
constexpr auto INFO_CFG = InitBuildConfig(LogLevel::LOG_INFO)
    .SetForeground(COLOR_GREEN)
    .Build();
constexpr auto WARNING_CFG = InitBuildConfig(LogLevel::LOG_WARNING)
    .SetForeground(COLOR_YELLOW)
    .Build();
constexpr auto ERROR_CFG = InitBuildConfig(LogLevel::LOG_ERROR)
    .SetForeground(COLOR_RED)
    .Build();
constexpr auto FATAL_CFG = InitBuildConfig(LogLevel::LOG_FATAL)
    .SetForeground(COLOR_WHITE)
    .SetBackground(COLOR_RED)
    .Build();

}
