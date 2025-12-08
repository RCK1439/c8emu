#include "rklog/rklog.hpp"

#include <memory>

namespace rklog {

std::shared_ptr<Logger> GlobalLogger::s_Logger;

void GlobalLogger::InitConsole(std::string_view title) noexcept
{
    if (s_Logger)
    {
        return;
    }

    s_Logger = std::make_shared<ConsoleLogger>(title);
}

void GlobalLogger::InitConsole(std::string_view title, LogStyle style) noexcept
{
    if (s_Logger)
    {
        return;
    }

    s_Logger = std::make_shared<ConsoleLogger>(title, style);
}

void GlobalLogger::InitFile(std::filesystem::path fileName, std::string_view title)
{
    if (s_Logger)
    {
        return;
    }

    s_Logger = std::make_shared<FileLogger>(fileName, title);
}

void GlobalLogger::InitFile(std::filesystem::path fileName, std::string_view title, LogStyle style)
{
    if (s_Logger)
    {
        return;
    }

    s_Logger = std::make_shared<FileLogger>(fileName, title, style);
}

}
