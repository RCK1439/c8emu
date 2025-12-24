#pragma once

#include "../Config/Level.hpp"
#include "../Config/Style.hpp"

#include <format>
#include <optional>
#include <string>

namespace rklog {

/**
 * Base class for every logger
 */
class Logger
{
public:
    constexpr Logger() = default;

    /**
     * Creates an instance of a logger with a title
     *
     * @param[in] title
     *      The title of the logger
     */
    constexpr Logger(std::string_view title) noexcept :
        m_Title(title) {}

    /**
     * Creates an instance of a logger with a custom style
     *
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr Logger(LogStyle style) noexcept :
        m_Style(style) {}
    
    /**
     * Creates an instance of a logger with a title and a custom style
     *
     * @param[in] title
     *      The title of the logger
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr Logger(std::string_view title, LogStyle style) noexcept :
        m_Title(title), m_Style(style) {}

    /**
     * Logs a message to `stderr` with a debug log level
     *
     * @param[in] fmt
     *      The format of the message
     * @param[in] args
     *      Any variadic arguments passed to the function
     */
    template<typename ... Args>
    void Debug(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_DEBUG);
    }

    /**
     * Logs a message to `stderr` with an info log level
     *
     * @param[in] fmt
     *      The format of the message
     * @param[in] args
     *      Any variadic arguments passed to the function
     */
    template<typename ... Args>
    void Info(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_INFO);
    }

    /**
     * Logs a message to `stderr` with a warning log level
     *
     * @param[in] fmt
     *      The format of the message
     * @param[in] args
     *      Any variadic arguments passed to the function
     */
    template<typename ... Args>
    void Warn(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_WARNING);
    }

    /**
     * Logs a message to `stderr` with an error log level
     *
     * @param[in] fmt
     *      The format of the message
     * @param[in] args
     *      Any variadic arguments passed to the function
     */
    template<typename ... Args>
    void Error(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_ERROR);
    }

    /**
     * Logs a message to `stderr` with a fatal log level
     *
     * @param[in] fmt
     *      The format of the message
     * @param[in] args
     *      Any variadic arguments passed to the function
     */
    template<typename ... Args>
    void Fatal(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_FATAL);
    }

protected:
    /**
     * Internal implementation of the logger
     *
     * @param[in] msg
     *      The message to log. Assumed to be already formatted
     * @param[in] level
     *      The log level severity to log the message with
     */
    virtual void LogInternal(std::string_view msg, LogLevel level) = 0;

protected:
    /// The title of the logger
    std::optional<std::string> m_Title = std::nullopt;
    /// The styling of the logger
    LogStyle m_Style = defaults::DEFAULT_STYLE;
};

}