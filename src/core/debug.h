#ifndef DEBUG_H
#define DEBUG_H

#include "platform.h"

#include <stdarg.h>

// --- assert macro -----------------------------------------------------------

#if defined(C8_DEBUG)
#   define C8_ASSERT(expr, ...) if (!(expr)) c8Panic(ERR_ASSERTION_FAILED, __VA_ARGS__)
#else
#   define C8_ASSERT(expr, ...) (void)0
#endif

// --- logging macros ---------------------------------------------------------

#if defined(C8_DEBUG)
#   define C8_LOG_INFO(...)    c8LogInfo(__VA_ARGS__)
#   define C8_LOG_WARNING(...) c8LogWarning(__VA_ARGS__)
#   define C8_LOG_ERROR(...)   c8LogError(__VA_ARGS__)
#   define C8_LOG_FATAL(...)   c8LogFatal(__VA_ARGS__)
#else
#   define C8_LOG_INFO(...)    (void)0
#   define C8_LOG_WARNING(...) (void)0
#   define C8_LOG_ERROR(...)   (void)0
#   define C8_LOG_FATAL(...)   (void)0
#endif

// --- type definitions -------------------------------------------------------

/**
 * Enum describing different error codes that the program can return if a panic
 * occurs
 */
typedef enum C8ErrorCode
{
    ERR_NONE,
    ERR_ASSERTION_FAILED,
    ERR_FAILED_TO_OPEN_FILE,
    ERR_FAILED_TO_READ_ROM,
    ERR_OUT_OF_MEMORY,
    ERR_FAILED_TO_LOAD_TARGET,
    ERR_INVALID_ADDRESS_MODE,
} C8ErrorCode;

// --- debug interface --------------------------------------------------------

/**
 * Initializes the logging system. This is to be called in debug builds only
 */
void c8InitLogging(void);

/**
 * Closes the logging system and any resources used by the logging system
 */
void c8CloseLogging(void);

/**
 * Prints the input text to `stderr` with an 'info' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void c8LogInfo(const char *fmt, ...);

/**
 * Prints the input text to `stderr` with an 'warning' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void c8LogWarning(const char *fmt, ...);

/**
 * Prints the input text to `stderr` with an 'error' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void c8LogError(const char *fmt, ...);

/**
 * Prints the input text to `stderr` with an 'fatal' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void c8LogFatal(const char *fmt, ...);

/**
 * Prints the input text and arguments to `stderr` with an 'info' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The arguments from the format specifier
 */
void c8LogInfoArgs(const char *fmt, va_list args);

/**
 * Prints the input text and arguments to `stderr` with an 'warning' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The arguments from the format specifier
 */
void c8LogWarningArgs(const char *fmt, va_list args);

/**
 * Prints the input text and arguments to `stderr` with an 'error' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The arguments from the format specifier
 */
void c8LogErrorArgs(const char *fmt, va_list args);

/**
 * Prints the input text and arguments to `stderr` with an 'fatal' tag
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The arguments from the format specifier
 */
void c8LogFatalArgs(const char *fmt, va_list args);

/**
 * Forcefully terminates the program in a panicked state
 *
 * @param[in] code
 *      The error code the panic occured with
 * @param[in] fmt
 *      The format specifier of the termination message
 */
void c8Panic(C8ErrorCode code, const char *fmt, ...);

#endif /* DEBUG_H */

