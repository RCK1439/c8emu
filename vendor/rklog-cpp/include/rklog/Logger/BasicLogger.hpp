#pragma once

#include "Logger.hpp"

#include "../Config/Style.hpp"

namespace rklog {

/**
 * Class acting as an interface for basic logging to the terminal
 */
class BasicLogger final : public Logger
{
public:
    constexpr BasicLogger() noexcept :
        Logger() {}
    
    /**
     * Creates an instance of a basic logger with a title
     *
     * @param[in] title
     *      The title of the logger
     */
    constexpr BasicLogger(std::string_view title) noexcept :
        Logger(title) {}

    /**
     * Creates an instance of a basic logger with a custom style
     *
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr BasicLogger(LogStyle style) noexcept :
        Logger(style) {}

    /**
     * Creates an instance of a basic logger with a title and a custom style
     *
     * @param[in] title
     *      The title of the logger
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr BasicLogger(std::string_view title, LogStyle style) noexcept :
        Logger(title, style) {}

protected:
    virtual void LogInternal(std::string_view msg, LogLevel level) override;
};

}