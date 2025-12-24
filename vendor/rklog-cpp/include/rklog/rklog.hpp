#pragma once

#include "Logger/BasicLogger.hpp"
#include "Logger/ColorLogger.hpp"
#include "Logger/FileLogger.hpp"

namespace rklog {

/**
 * Gets the global instance of a basic logger
 *
 * @param[in] title
 *      The title to give the basic logger
 *
 * @return
 *      The global basic logger instance
 */
BasicLogger& GetBasicLogger(std::string_view title = "global") noexcept;

/**
 * Gets the global instance of a color logger
 *
 * @param[in] title
 *      The title to give the global logger
 *
 * @return
 *      The global color logger instance
 */
ColorLogger& GetColorLogger(std::string_view title = "global") noexcept;

}
