#include "../include/rklog/core/logger.hpp"
#include "../include/rklog/core/platform.hpp"

#if defined(RKLOG_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define ENABLE_COLORED_CONSOLE() EnableColoredConsoleWindows()
#else
#define ENABLE_COLORED_CONSOLE() (void)0
#endif

namespace rklog {

    static void EnableColoredConsoleWindows() noexcept
    {
        const HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hErr, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwMode);
    }

    ConsoleLogger::ConsoleLogger(std::string_view title) noexcept :
        Logger(title)
    {
        ENABLE_COLORED_CONSOLE();
    }

    ConsoleLogger::ConsoleLogger(std::string_view title, LogStyle style) noexcept :
        Logger(title, style)
    {
        ENABLE_COLORED_CONSOLE();
    }

}