#pragma once

#include "config.hpp"
#include "level.hpp"
#include "platform.hpp"

#include <string_view>

namespace rklog {

/**
 * Class representing the style of the log messages
 */
class LogStyle final
{
public:
    /**
     * Creates the default style for logging
     */
    constexpr LogStyle() noexcept :
        m_CfgInfo(DEFAULT_INFO_TAG, GREEN_COLOR),
        m_CfgWarning(DEFAULT_WARNING_TAG, YELLOW_COLOR),
        m_CfgError(DEFAULT_ERROR_TAG, RED_COLOR),
        m_CfgFatal(DEFAULT_FATAL_TAG, WHITE_COLOR, RED_COLOR)
    {}

    /**
     * Creates an instance of `LogStyle` with different configurations for
     * different log severities
     *
     * @param cfgInfo
     *      The configuration for info log level
     * @param cfgWarning
     *      The configuration for warning log level
     * @param cfgError
     *      The configuration for error log level
     * @param cfgFatal
     *      The configuration for fatal log level
     */
    constexpr LogStyle(LogConfig cfgInfo, LogConfig cfgWarning, LogConfig cfgError, LogConfig cfgFatal) noexcept :
        m_CfgInfo(cfgInfo), m_CfgWarning(cfgWarning), m_CfgError(cfgError), m_CfgFatal(cfgFatal) {}

    /**
     * Gets the configuration for `lvl`
     *
     * @param lvl
     *      The log level severity corresponding to the log configuration
     */
    constexpr LogConfig GetConfig(LogLevel lvl) const noexcept
    {
        switch (lvl)
        {
            case LogLevel::LOG_INFO: return m_CfgInfo;
            case LogLevel::LOG_WARNING: return m_CfgWarning;
            case LogLevel::LOG_ERROR: return m_CfgError;
            case LogLevel::LOG_FATAL: return m_CfgFatal;
        }

        RKLOG_UNREACHABLE();
    }

private:
    static constexpr std::string_view DEFAULT_INFO_TAG = "INFO";
    static constexpr std::string_view DEFAULT_WARNING_TAG = "WARNING";
    static constexpr std::string_view DEFAULT_ERROR_TAG = "ERROR";
    static constexpr std::string_view DEFAULT_FATAL_TAG = "FATAL";

    static constexpr Color GREEN_COLOR = Color(0x00, 0xFF, 0x00);  // Green
    static constexpr Color YELLOW_COLOR = Color(0xFF, 0xFF, 0x00); // Yellow
    static constexpr Color RED_COLOR = Color(0xFF, 0x00, 0x00);    // Red
    static constexpr Color WHITE_COLOR = Color(0xFF, 0xFF, 0xFF);  // White

private:
    const LogConfig m_CfgInfo;    // The configuration for info logs
    const LogConfig m_CfgWarning; // The configuration for warning logs
    const LogConfig m_CfgError;   // The configuration for error logs
    const LogConfig m_CfgFatal;   // The configuration for fatal logs
};
    
}
