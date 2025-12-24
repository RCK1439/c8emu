#pragma once

#include "Logger.hpp"

#include "../Config/Style.hpp"

namespace rklog {

/**
 * Class acting as an interface for color logging to the terminal
 */
class ColorLogger final : public Logger
{
public:
    constexpr ColorLogger() noexcept :
        Logger() {}
    
    /**
     * Creates an instance of a color logger with a title
     *
     * @param[in] title
     *      The title of the logger
     */
    constexpr ColorLogger(std::string_view title) noexcept :
        Logger(title) {}
    
        /**
     * Creates an instance of a color logger with a custom style
     *
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr ColorLogger(LogStyle style) noexcept :
        Logger(style) {}

    /**
     * Creates an instance of a color logger with a title and a custom style
     *
     * @param[in] title
     *      The title of the logger
     * @param[in] style
     *      The custom style of the logger
     */
    constexpr ColorLogger(std::string_view title, LogStyle style) noexcept :
        Logger(title, style) {}

protected:
    virtual void LogInternal(std::string_view msg, LogLevel lvl) override;
};

}