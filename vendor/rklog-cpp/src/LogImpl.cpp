#include "rklog/Logger/BasicLogger.hpp"
#include "rklog/Logger/ColorLogger.hpp"
#include "rklog/Logger/FileLogger.hpp"

#include "rklog/Core/Platform.hpp"
#include "rklog/Core/Time.hpp"

#include <iostream>

#if defined(RKLOG_PLATFORM_WINDOWS)
#include <Windows.h>

static void EnableVirtualConsole()
{
    static bool enabled = false;
    if (enabled)
    {
        return;
    }

    const ::HANDLE hErr = ::GetStdHandle(STD_ERROR_HANDLE);
    ::DWORD dwMode = 0;
    ::GetConsoleMode(hErr, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    ::SetConsoleMode(hErr, dwMode);

    enabled = true;
}

#endif // Windows configuration

namespace rklog {

static std::string BuildLogMessage(const std::optional<std::string>& loggerTitle, const LogConfig& cfg, std::string_view msg)
{
    const auto tag = cfg.GetTag();
    const auto ts = TimeStamp::Now();

    if (loggerTitle.has_value())
    {
        const auto title = loggerTitle.value();
        return std::format("[{}]:[{}]:[{}]: {}", title, tag, ts, msg);
    }
    else
    {
        return std::format("[{}]:[{}]: {}", tag, ts, msg);
    }
}

static std::optional<std::string> BuildColorCode(std::optional<Color> fg, std::optional<Color> bg)
{
    if (fg.has_value() && bg.has_value())
    {
        const Color fgColor = fg.value();
        const Color bgColor = bg.value();
        return std::format("\033[38;2;{};{};{};48;2;{};{};{}m", fgColor.r, fgColor.g, fgColor.b, bgColor.r, bgColor.g, bgColor.b);
    }
    else if (fg.has_value())
    {
        const Color fgColor = fg.value();
        return std::format("\033[38;2;{};{};{}m", fgColor.r, fgColor.g, fgColor.b);
    }
    else if (bg.has_value())
    {
        const Color bgColor = bg.value();
        return std::format("\033[48;2;{};{};{}m", bgColor.r, bgColor.g, bgColor.b);
    }

    return std::nullopt;
}

static std::string ColorizeString(std::string_view str, std::optional<Color> fg, std::optional<Color> bg)
{
    constexpr std::string_view ANSI_RESET = "\033[0m";

    if (const auto colorCode = BuildColorCode(fg, bg))
    {
        const auto ansiColor = colorCode.value();
        return std::format("{}{}{}", ansiColor, str, ANSI_RESET);
    }

    return str.data();
}

void BasicLogger::LogInternal(std::string_view msg, LogLevel level)
{
    const auto cfg = m_Style.GetConfig(level);
    const auto logMessage = BuildLogMessage(m_Title, cfg, msg);

    std::println(std::cerr, "{}", logMessage);
}

void ColorLogger::LogInternal(std::string_view msg, LogLevel level)
{
    const auto cfg = m_Style.GetConfig(level);
    const auto logMessage = BuildLogMessage(m_Title, cfg, msg);
    const auto coloredLogMessage = ColorizeString(logMessage, cfg.GetForegroundColor(), cfg.GetBackgroundColor());

#if defined(RKLOG_PLATFORM_WINDOWS)
    EnableVirtualConsole();
#endif

    std::println(std::cerr, "{}", coloredLogMessage);
}

void FileLogger::LogInternal(std::string_view msg, LogLevel level)
{
    const auto cfg = m_Style.GetConfig(level);
    const auto logMessage = BuildLogMessage(m_Title, cfg, msg);
    std::println(m_FileHandle, "{}", logMessage);
    
    if (m_WriteToStdErr)
    {
#if defined(RKLOG_PLATFORM_WINDOWS)
        EnableVirtualConsole();
#endif
        const auto coloredLogMessage = ColorizeString(logMessage, cfg.GetForegroundColor(), cfg.GetBackgroundColor());
        std::println(std::cerr, "{}", coloredLogMessage);
    }
}

ColorLogger& GetColorLogger(std::string_view title = "global") noexcept
{
    static ColorLogger colorLogger(title);
    return colorLogger;
}

BasicLogger& GetBasicLogger(std::string_view title = "global") noexcept
{
    static BasicLogger basicLogger(title);
    return basicLogger;
}

}