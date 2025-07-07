/**
 * rklog is a header-only logging library used for clear message logging/debugging
 * 
 * version: 1.2
 * author: Ruan C. Keet
 */

#ifndef RK_LOG_H
#define RK_LOG_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

// --- literal macros ---------------------------------------------------------

#define C_LITERAL(type) (type)

#define RK_COLOR(R, G, B)\
    C_LITERAL(rkColor) { \
        .r = (uint8_t)R, \
        .g = (uint8_t)G, \
        .b = (uint8_t)B  \
    }                    \

#define RK_CONFIG(TAG, BG_COLOR, FG_COLOR)\
    C_LITERAL(rkLogConfig) {              \
        .tag = TAG,                       \
        .background = BG_COLOR,           \
        .foreground = FG_COLOR,           \
    }                                     \

#define RK_STYLE(CFG_INFO, CFG_WARNING, CFG_ERROR, CFG_FATAL_ERROR)\
    C_LITERAL(rkLogStyle) {                                        \
        .cfgInfo = CFG_INFO,                                       \
        .cfgWarning = CFG_WARNING,                                 \
        .cfgError = CFG_ERROR,                                     \
        .cfgFatalError = CFG_FATAL_ERROR,                          \
    }                                                              \

#define RK_COLOR_GREEN  RK_COLOR(0, 255, 0)
#define RK_COLOR_RED    RK_COLOR(255, 0, 0)
#define RK_COLOR_YELLOW RK_COLOR(255, 255, 0)
#define RK_COLOR_WHITE  RK_COLOR(255, 255, 255)
#define RK_COLOR_BLACK  RK_COLOR(0, 0, 0)

#define RK_NO_BG_COL RK_COLOR_BLACK
#define RK_NO_FG_COL RK_COLOR_WHITE

// --- defaults ---------------------------------------------------------------

#define RK_DEFAULT_INFO_CFG C_LITERAL(rkLogConfig) {\
        .tag = "INFO",                              \
        .background = RK_COLOR_BLACK,               \
        .foreground = RK_COLOR_GREEN,               \
    }                                               \

#define RK_DEFAULT_WARNING_CFG (rkLogConfig) {\
        .tag = "WARNING",                     \
        .background = RK_COLOR_BLACK,         \
        .foreground = RK_COLOR_YELLOW,        \
    }                                         \

#define RK_DEFAULT_ERROR_CFG (rkLogConfig) {\
        .tag = "ERROR",                     \
        .background = RK_COLOR_BLACK,       \
        .foreground = RK_COLOR_RED,         \
    }                                       \

#define RK_DEFAULT_FATAL_CFG (rkLogConfig) {\
        .tag = "FATAL",                     \
        .background = RK_COLOR_RED,         \
        .foreground = RK_COLOR_WHITE,       \
    }                                       \

#define RK_DEFAULT_LOG_STYLE (rkLogStyle) {\
    .cfgInfo = RK_DEFAULT_INFO_CFG,        \
    .cfgWarning = RK_DEFAULT_WARNING_CFG,  \
    .cfgError = RK_DEFAULT_ERROR_CFG,      \
    .cfgFatalError = RK_DEFAULT_FATAL_CFG, \
}                                          \

// --- type definitions -------------------------------------------------------

/**
 * Struct describing 24-bit color for use with the ANSI codes
 */
typedef struct rkColor
{
    uint8_t r; // red channel
    uint8_t g; // green channel
    uint8_t b; // blue channel
} rkColor;

/**
 * Struct holding color information relating to log messages
 */
typedef struct rkLogConfig
{
    const char *tag;        // tag title for the severity level
    rkColor     background; // background color
    rkColor     foreground; // foreground color
} rkLogConfig;

/**
 * Struct containing the overall style of a logger. Mainly just holds color
 * and tag information of each of the log types.
 */
typedef struct rkLogStyle
{
    rkLogConfig cfgInfo;       // the configuration for info logs
    rkLogConfig cfgWarning;    // the configuration for warning logs
    rkLogConfig cfgError;      // the configuration for error logs
    rkLogConfig cfgFatalError; // the configuration for fatal error logs
} rkLogStyle;

typedef struct rkLogger rkLogger;

// --- rklog interface --------------------------------------------------------

/**
 * Creates a new handle to a logger with default logging style and the provided
 * title
 *
 * @param[in] title
 *      The title of the logger
 *
 * @return
 *      A handle to the newly created logger
 */
rkLogger *rkDefaultLogger(const char *title);

/**
 * Creates a new handle to a file logger with default logging style and the
 * provided title
 *
 * @param[in] fileName
 *      The file path and name of the file to log to
 * @param[in] title
 *      The title of the logger
 *
 * @return
 *      A handle to the newly created logger
 */
rkLogger *rkDefaultFileLogger(const char *fileName, const char *title);

/**
 * Creates a new handle to a logger with custom logging style
 * specifications
 *
 * @param[in] title
 *      The title of this logger
 * @param[in] style
 *      The configuration of all the log types
 *
 * @return
 *      A handle to the newly created logger
 */
rkLogger *rkCreateLogger(const char *title, rkLogStyle style);

/**
 * Creates a new handle a new handle to a file logger with custom logging style
 * specifications
 *
 * @param[in] title
 *      The title of this logger
 * @param[in] style
 *      The configuration of all the log types
 *
 * @return
 *      A handle to the newly created logger
 */
rkLogger *rkCreateFileLogger(const char *fileName, const char *title, rkLogStyle style);

/**
 * Closes the logger and frees all resources used by the logger.
 *
 * @param[in] logger
 *      The handle of the logger to close
 */
void rkCloseLogger(rkLogger *logger);

/**
 * Logs a formatted message with an info tag to `stdout`
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogInfo(rkLogger *logger, const char *fmt, ...);

/**
 * Logs a formatted message with a warning tag to `stdout`
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogWarning(rkLogger *logger, const char *fmt, ...);

/**
 * Logs a formatted message with an error tag to `stdout`
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogError(rkLogger *logger, const char *fmt, ...);

/**
 * Logs a formatted message with a fatal error tag to `stdout`
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogFatal(rkLogger *logger, const char *fmt, ...);

/**
 * Logs a formatted message with an info tag to `stdout` with explicit
 * arguments
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogInfoArgs(rkLogger *logger, const char *fmt, va_list args);

/**
 * Logs a formatted message with a warning tag to `stdout` with explicit
 * arguments
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogWarningArgs(rkLogger *logger, const char *fmt, va_list args);

/**
 * Logs a formatted message with an error tag to `stdout` with explicit
 * arguments
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogErrorArgs(rkLogger *logger, const char *fmt, va_list args);

/**
 * Logs a formatted message with a fatal error tag to `stdout` with explicit
 * arguments
 *
 * @param[in] logger
 *      A handle to the logger
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogFatalArgs(rkLogger *logger, const char *fmt, va_list args);

#if defined(RK_LOG_IMPLEMENTATION)

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- string tokens ----------------------------------------------------------

#define RK_TOKEN_ESCAPE_CODE_START "\033["
#define RK_TOKEN_ESCAPE_CODE_END   "m"

#define RK_TOKEN_ESCAPE_CODE_RESET RK_TOKEN_ESCAPE_CODE_START "0" RK_TOKEN_ESCAPE_CODE_END

#define RK_TOKEN_FOREGROUND "38;"
#define RK_TOKEN_BACKGROUND "48;"

#define RK_TOKEN_TRUE_COLOR_MODE "2;"
#define RK_TOKEN_256_COLOR_PALETTE_MODE "5;"

// --- formats ----------------------------------------------------------------

//                                                        foreground
//                                                        | true color mode
//                                                        | |  red channel
//                                                        | |  |  green channel
//                                                        | |  |  |  blue channel
//                                                        | |  |  |  |
#define RK_FMT_COLOR_PRELUDE RK_TOKEN_ESCAPE_CODE_START "48;2;%d;%d;%d;38;2;%d;%d;%d" RK_TOKEN_ESCAPE_CODE_END
//                                                                      | |  |  |  |
//                                                                      | |  |  |  blue channel
//                                                                      | |  |  green channel
//                                                                      | |  red channel
//                                                                      | true color mode
//                                                                      background

#define RK_FMT_LABEL "[%s]:[%s]:[%02d:%02d:%02d]: "
//                      |    |      |    |    |
//                      |    |      |    |    seconds
//                      |    |      |    minutes
//                      |    |      hours
//                      |    log severity
//                      title

#define RK_FMT_COLOR_OUTPUT "%s%s%s" RK_TOKEN_ESCAPE_CODE_RESET "\n"
//                            | | |
//                            | | log message
//                            | log label
//                            log prelude

#define RK_FMT_OUTPUT "%s%s\n"
//                      | |
//                      | log message
//                      log label

// --- constants --------------------------------------------------------------

#define MAX_LOGGER_TITLE_SIZE 64

#define MAX_PRELUDE_SIZE  64
#define MAX_LABEL_SIZE    (64 + MAX_LOGGER_TITLE_SIZE)
#define MAX_MESSAGE_SIZE  256

// --- type definitions -------------------------------------------------------

/**
 * Struct holding the system time at the point the log occured
 */
typedef struct rkTimeStamp
{
    uint32_t hours;   //--+
    uint32_t minutes; //  +-- pretty self explanatory
    uint32_t seconds; //--+
} rkTimeStamp;

/**
 * Struct containing the settings of the different log messages
 */
struct rkLogger
{
    char        title[MAX_LOGGER_TITLE_SIZE+1]; // the title for this logger
    rkLogStyle  style;                          // color configurations for each severity level
    FILE       *output;                         // output stream for the logging messages
};

// --- utility functions ------------------------------------------------------

/**
 * Allocates and creates a new logger handle
 *
 * @param[in] out
 *      The output stream handle of the logger
 * @param[in] title
 *      The title of the logger
 * @param[in] style
 *      The style specification of the logger
 */
static rkLogger *rkNewLogger(FILE *out, const char *title, rkLogStyle style);

/**
 * Logs the formatted message with its configuration to `stdout`
 *
 * @param[in] out
 *      The output stream to log to
 * @param[in] title
 *      The title of the logger
 * @param[in] cfg
 *      The configuration in which the colors of how the log message must look
 * @param[in] fmt
 *      The format of the message
 * @param[in] args
 *      Variadic arguments
 */
static void rkLogInternal(FILE *out, const char *title, rkLogConfig cfg, const char *fmt, va_list args);

/**
 * Formats `buf` to be the colored log message prelude
 *
 * @param[out] buf
 *      The buffer to format according to the prelude
 * @param[in] len
 *      The maximum length of the buffer
 * @param[in] background
 *      The background color of the text
 * @param[in] foreground
 *      The foreground color of the text
 */
static void rkGenColorPrelude(char *buf, size_t len, rkColor background, rkColor foreground);

/**
 * Formats `buf` to contain the log message tag and timestamp
 *
 * @param[out] buf
 *      The buffer to format according to the label
 * @param[in] len
 *      The maximum length of the buffer
 * @param[in] tag
 *      The severity tag of the log message
 * @param[in] title
 *      The title of the logger
 */
static void rkGenLabel(char *buf, size_t len, const char *tag, const char *title);

/**
 * Queries the system's current local time
 *
 * @return
 *      The system time
 */
static rkTimeStamp rkGetCurrentTime(void);

// --- utility function implementation ----------------------------------------

static rkLogger *rkNewLogger(FILE *out, const char *title, rkLogStyle style)
{
    rkLogger *const logger = (rkLogger *)malloc(sizeof(rkLogger));
    if (!logger)
    {
        return NULL;
    }

    strncpy(logger->title, title, MAX_LOGGER_TITLE_SIZE);
    logger->style = style;
    logger->output = out;

    return logger;
}

static void rkLogInternal(FILE *out, const char *title, rkLogConfig cfg, const char *fmt, va_list args)
{
    char label[MAX_LABEL_SIZE+1] = { 0 };
    char message[MAX_MESSAGE_SIZE+1] = { 0 };
    
    rkGenLabel(label, MAX_LABEL_SIZE, title, cfg.tag);
    vsnprintf(message, MAX_MESSAGE_SIZE, fmt, args);
    
    if (out == stderr)
    {
        char prelude[MAX_PRELUDE_SIZE+1] = { 0 };
        rkGenColorPrelude(prelude, MAX_PRELUDE_SIZE, cfg.foreground, cfg.background);

        fprintf(out, RK_FMT_COLOR_OUTPUT, prelude, label, message);
    }
    else
    {
        fprintf(out, RK_FMT_OUTPUT, label, message);
    }
}

static void rkGenColorPrelude(char *buf, size_t len, rkColor background, rkColor foreground)
{
    snprintf(buf, len, RK_FMT_COLOR_PRELUDE,
        foreground.r, foreground.g, foreground.b,
        background.r, background.g, background.b
    );
}

static void rkGenLabel(char *buf, size_t len, const char *title, const char *tag)
{
    const rkTimeStamp timeStamp = rkGetCurrentTime();   
    snprintf(buf, len, RK_FMT_LABEL, title, tag, timeStamp.hours, timeStamp.minutes, timeStamp.seconds);
}

static rkTimeStamp rkGetCurrentTime(void)
{
    struct tm *timeInfo = NULL;

    const time_t now = time(NULL);
    timeInfo = localtime(&now);
    
    const rkTimeStamp currTime = {
        .hours = (uint32_t)timeInfo->tm_hour,
        .minutes = (uint32_t)timeInfo->tm_min,
        .seconds = (uint32_t)timeInfo->tm_sec,
    };

    return currTime;
}

// --- rklog implementation ---------------------------------------------------

rkLogger *rkDefaultLogger(const char *title)
{
    return rkCreateLogger(title, RK_DEFAULT_LOG_STYLE);
}

rkLogger *rkDefaultFileLogger(const char *fileName, const char *title)
{
    return rkCreateFileLogger(fileName, title, RK_DEFAULT_LOG_STYLE);
}

rkLogger *rkCreateFileLogger(const char *fileName, const char *title, rkLogStyle style)
{
    FILE *out = fopen(fileName, "w");
    if (!out)
    {
        return NULL;
    }

    return rkNewLogger(out, title, style);
}

rkLogger *rkCreateLogger(const char *title, rkLogStyle style)
{
    return rkNewLogger(stderr, title, style);
}

void rkCloseLogger(rkLogger *logger)
{
    if (logger->output != stderr)
    {
        fclose(logger->output);
    }

    free(logger);
}

void rkLogInfo(rkLogger *logger, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogInfoArgs(logger, fmt, args);
    va_end(args);
}

void rkLogWarning(rkLogger *logger, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogWarningArgs(logger, fmt, args);
    va_end(args);
}

void rkLogError(rkLogger *logger, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogErrorArgs(logger, fmt, args);
    va_end(args);
}

void rkLogFatal(rkLogger *logger, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogFatalArgs(logger, fmt, args);
    va_end(args);
}

void rkLogInfoArgs(rkLogger *logger, const char *fmt, va_list args)
{
    rkLogInternal(logger->output, logger->title, logger->style.cfgInfo, fmt, args);
}

void rkLogWarningArgs(rkLogger *logger, const char *fmt, va_list args)
{
    rkLogInternal(logger->output, logger->title, logger->style.cfgWarning, fmt, args);
}

void rkLogErrorArgs(rkLogger *logger, const char *fmt, va_list args)
{
    rkLogInternal(logger->output, logger->title, logger->style.cfgError, fmt, args);
}

void rkLogFatalArgs(rkLogger *logger, const char *fmt, va_list args)
{
    rkLogInternal(logger->output, logger->title, logger->style.cfgFatalError, fmt, args);
}

#endif /* RK_LOG_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#endif /* RK_LOG_H */
