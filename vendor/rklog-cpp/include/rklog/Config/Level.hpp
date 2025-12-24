#pragma once

#include <cstdint>

namespace rklog {

/**
 * Enum describing the different log levels of the logs
 */
enum class LogLevel : uint8_t
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
};

}
