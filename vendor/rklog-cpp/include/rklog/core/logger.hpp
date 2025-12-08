#pragma once

#include "level.hpp"
#include "time.hpp"
#include "style.hpp"
#include "platform.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#if defined(RKLOG_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace rklog {

// --- format specifiers ------------------------------------------------------

#define RKLOG_FMT_COLOR_OUTPUT "{}{}{}" RKLOG_ESC_CODE_RESET
//                               | | |
//                               | | log message
//                               | log label
//                               log prelude

#define RKLOG_FMT_OUTPUT "{}{}"
//                         | |
//                         | log message
//                         log label

// --- type definitions -------------------------------------------------------

/**
 * Base class for the logger
 */
class Logger
{
public:
    /**
     * Creates the base for a Logger with a title and default styling
     *
     * @param title
     *      The title of the logger
     */
    constexpr explicit Logger(std::string_view title) noexcept :
        m_Title(title) {}
    
    /**
     * Creates a logger with a title and custom styling
     *
     * @param title
     *      The title of the logger
     * @param style
     *      The style of the log messages
     */
    constexpr explicit Logger(std::string_view title, LogStyle style) noexcept :
        m_Title(title), m_Style(style) {}

    /**
     * Logs a message to the output stream with an info severity tag.
     *
     * @param fmt
     *      The format specifier of the message
     * @param args
     *      The arguments of the message
     */
    template<typename ... Args>
    void Info(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_INFO);
    }

    /**
     * Logs a message to the output stream with a warning severity tag.
     *
     * @param fmt
     *      The format specifier of the message
     * @param args
     *      The arguments of the message
     */
    template<typename ... Args>
    void Warn(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_WARNING);
    }

    /**
     * Logs a message to the output stream with an error severity tag.
     *
     * @param fmt
     *      The format specifier of the message
     * @param args
     *      The arguments of the message
     */
    template<typename ... Args>
    void Error(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_ERROR);
    }

    /**
     * Logs a message to the output stream with a fatal severity tag.
     *
     * @param fmt
     *      The format specifier of the message
     * @param args
     *      The arguments of the message
     */
    template<typename ... Args>
    void Fatal(const std::format_string<Args...> fmt, Args&& ... args)
    {
        const std::string msg = std::format(fmt, std::forward<Args>(args)...);
        LogInternal(msg, LogLevel::LOG_FATAL);
    }

protected:
    /**
     * Internal implementation of the log message
     *
     * @param msg
     *      The message to log
     * @param lvl
     *      The severity of the log message
     */
    virtual void LogInternal(std::string_view msg, LogLevel lvl) = 0;

protected:
    const std::string m_Title; // The title of the logger
    const LogStyle    m_Style; // The style of the logger
};

// --- console logger ---------------------------------------------------------

/**
 * A logger that logs to the terminal via `stderr`
 */
class ConsoleLogger final : public Logger
{
public:
    /**
     * Creates an instance of the `ConsoleLogger` with default styling
     *
     * @param title
     *      The title of the logger
     */
    explicit ConsoleLogger(std::string_view title) noexcept :
        Logger(title)
    {
#if defined(RKLOG_PLATFORM_WINDOWS)
        const HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hErr, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwMode);
#endif
    }

    /**
     * Creates an instance of the `ConsoleLogger` with custom styling
     *
     * @param title
     *      The title of the logger
     * @param style
     *      The style of the log messages
     */
    explicit ConsoleLogger(std::string_view title, LogStyle style) noexcept :
        Logger(title, style)
    {
#if defined(RKLOG_PLATFORM_WINDOWS)
        const HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hErr, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwMode);
#endif
    }

protected:
    /**
     * Logs `msg` to `stderr` with `lvl` severity
     *
     * @param msg
     *      The message to log
     * @param lvl
     *      The severity level of the log message
     */
    virtual void LogInternal(std::string_view msg, LogLevel lvl) override
    {
        const LogConfig cfg = m_Style.GetConfig(lvl);

        const std::string label = cfg.GenerateLabel(m_Title, TimeStamp::Now());
        const std::string colorPrelude = cfg.GenerateColorPrelude();

#if defined(RKLOG_COMPILER_GCC)
        std::cerr << std::format(RKLOG_FMT_COLOR_OUTPUT, colorPrelude, label, msg) << '\n';
#else
        std::println(std::cerr, RKLOG_FMT_COLOR_OUTPUT, colorPrelude, label, msg);
#endif
    }
};

// --- file logger ------------------------------------------------------------

/**
 * A logger that logs to a file in a text format
 */
class FileLogger final : public Logger
{
public:
    FileLogger() = delete;

    /**
     * Creates an instance of the `FileLogger` with default styling
     *
     * @param fileName
     *      The name of the file to log to
     * @param title
     *      The title of the logger
     */
    explicit FileLogger(std::filesystem::path fileName, std::string_view title) :
        Logger(title), m_FileHandle(fileName) {}
    
    /**
     * Creates an instance of the `FileLogger` with custom styling
     *
     * @param fileName
     *      The name of the file to log to
     * @param title
     *      The title of the logger
     * @param style
     *      The styling of the log messages
     */
    explicit FileLogger(std::filesystem::path fileName, std::string_view title, LogStyle style) :
        Logger(title, style), m_FileHandle(fileName) {}

protected:
    /**
     * Logs `msg` to the file with `lvl` severity
     *
     * @param msg
     *      The message to log to the file
     * @param lvl
     *      The severity of the log message
     */
    virtual void LogInternal(std::string_view msg, LogLevel lvl) override
    {
        const LogConfig cfg = m_Style.GetConfig(lvl);
        const std::string label = cfg.GenerateLabel(m_Title, TimeStamp::Now());

#if defined(RKLOG_COMPILER_GCC)
        m_FileHandle << std::format(RKLOG_FMT_OUTPUT, label, msg) << '\n';
#else
        std::println(m_FileHandle, RKLOG_FMT_OUTPUT, label, msg);
#endif
    }
    
private:
    std::ofstream m_FileHandle; // The handle to the file being logged to
};

}
