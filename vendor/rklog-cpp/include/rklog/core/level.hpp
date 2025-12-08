#pragma once

#include <cstdint>

namespace rklog {

/**
 * Enum representing the log level severity
 */
enum class LogLevel : uint8_t
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
};

}
