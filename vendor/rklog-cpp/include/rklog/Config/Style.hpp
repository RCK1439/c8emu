#pragma once

#include "Config.hpp"

#include "../Core/Platform.hpp"

namespace rklog {

/**
 * Class describing the style of a logger for its configurations
 */
class LogStyle final
{
public:
    /**
     * Gets the configuration based on the given log level
     *
     * @param[in] level
     *      The log level to get the configuration for
     *
     * @return
     *      The log configuration based on the log level
     */
    constexpr const LogConfig& GetConfig(LogLevel level) const noexcept
    {
        switch (level)
        {
            case LogLevel::LOG_DEBUG: return m_CfgDebug;
            case LogLevel::LOG_INFO: return m_CfgInfo;
            case LogLevel::LOG_WARNING: return m_CfgWarning;
            case LogLevel::LOG_ERROR: return m_CfgError;
            case LogLevel::LOG_FATAL: return m_CfgFatal;
        }

        RKLOG_UNREACHABLE();
    }

private:
    constexpr LogStyle() = default;

private:
    /// The configuration for debug logs
    LogConfig m_CfgDebug = defaults::DEBUG_CFG;
    /// The configuration for info logs
    LogConfig m_CfgInfo = defaults::INFO_CFG;
    /// The configuration for warning logs
    LogConfig m_CfgWarning = defaults::WARNING_CFG;
    /// The configuration for error logs
    LogConfig m_CfgError = defaults::ERROR_CFG;
    /// The configuration for fatal logs
    LogConfig m_CfgFatal = defaults::FATAL_CFG;

    friend class LogStyleBuilder;
};

/**
 * Class used for building log styles
 */
class LogStyleBuilder final
{
public:
    LogStyleBuilder(const LogStyleBuilder&) = delete;
    LogStyleBuilder(LogStyleBuilder&&) = delete;

    /**
     * Sets the configuration
     *
     * @param[in] cfg
     *      The configuration to set onto the style
     *
     * @return
     *      This instance of the builder
     */
    [[nodiscard]] constexpr LogStyleBuilder& SetConfig(const LogConfig& cfg) noexcept
    {
        switch (cfg.GetLevel())
        {
            case LogLevel::LOG_DEBUG:
                m_Style.m_CfgDebug = cfg;
                return *this;
            case LogLevel::LOG_INFO:
                m_Style.m_CfgInfo = cfg;
                return *this;
            case LogLevel::LOG_WARNING:
                m_Style.m_CfgWarning = cfg;
                return *this;
            case LogLevel::LOG_ERROR:
                m_Style.m_CfgError = cfg;
                return *this;
            case LogLevel::LOG_FATAL:
                m_Style.m_CfgFatal = cfg;
                return *this;
        }

        RKLOG_UNREACHABLE();
    }

    /**
     * Finalizes the build for the style
     *
     * @return
     *      The final log style
     */
    [[nodiscard]] constexpr LogStyle&& Build() noexcept
    {
        return std::move(m_Style);
    }

private:
    constexpr LogStyleBuilder() noexcept = default;

private:
    /// The style being built
    LogStyle m_Style;

    friend constexpr LogStyleBuilder InitBuildStyle() noexcept;
};

/**
 * Initializes the building of a log style
 *
 * @return
 *      An instance of the style builder 
 */
[[nodiscard]] constexpr LogStyleBuilder InitBuildStyle() noexcept
{
    return LogStyleBuilder();
}
    
}

namespace rklog::defaults {

constexpr LogStyle DEFAULT_STYLE = InitBuildStyle()
    .SetConfig(DEBUG_CFG)
    .SetConfig(INFO_CFG)
    .SetConfig(WARNING_CFG)
    .SetConfig(ERROR_CFG)
    .SetConfig(FATAL_CFG)
    .Build();

}
