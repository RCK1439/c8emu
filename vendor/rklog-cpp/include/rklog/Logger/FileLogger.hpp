#pragma once

#include "Logger.hpp"

#include "../Config/Style.hpp"

#include <fstream>
#include <filesystem>

namespace rklog {

/**
 * Class acting as an interface for file logging
 */
class FileLogger final : public Logger
{
public:
    /**
     * Creates an instance of a file logger
     *
     * @param[in] filePath
     *      The path to the file to log to
     */
    FileLogger(std::filesystem::path filePath) :
        Logger(), m_FileHandle(filePath) {}
    
    /**
     * Creates an instance of a file logger with a title
     *
     * @param[in] filePath
     *      The path to the file to log to
     * @param[in] title
     *      The title of the logger
     */
    FileLogger(std::filesystem::path filePath, std::string_view title) :
        Logger(title), m_FileHandle(filePath) {}
    
    /**
     * Creates an instance of a file logger with a custom style
     *
     * @param[in] filePath
     *      The path to the file to log to
     * @param[in] style
     *      The custom style of the logger
     */
    FileLogger(std::filesystem::path filePath, LogStyle style) :
        Logger(style), m_FileHandle(filePath) {}
    
    /**
     * Creates an instance of a file logger with a title and a custom style
     *
     * @param[in] filePath
     *      The path to the file to log to
     * @param[in] title
     *      The title of the logger
     * @param[in] style
     *      The custom style of the logger
     */
    FileLogger(std::filesystem::path filePath, std::string_view title, LogStyle style) :
        Logger(title, style), m_FileHandle(filePath) {}
    
    /**
     * Enables this logger to log to `stderr` as well
     */
    inline void EnableWriteToStdErr() noexcept
    {
        m_WriteToStdErr = true;
    }

    /**
     * Disables this logger from logging to `stderr`
     */
    inline void DisableWriteToStdErr() noexcept
    {
        m_WriteToStdErr = false;
    }

protected:
    virtual void LogInternal(std::string_view msg, LogLevel level) override;

private:
    /// The handle to the file that this logger is logging to
    std::ofstream m_FileHandle;
    /// A flag indicating whether the logger should log to `stderr` too
    bool m_WriteToStdErr = false;
};

}