#ifndef LOG_H
#define LOG_H

#include "platform.h"

#include <stdarg.h>

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

void c8InitLogging(void);
void c8CloseLogging(void);

void c8LogInfo(const char *fmt, ...);
void c8LogWarning(const char *fmt, ...);
void c8LogError(const char *fmt, ...);
void c8LogFatal(const char *fmt, ...);

void c8LogInfoArgs(const char *fmt, va_list args);
void c8LogWarningArgs(const char *fmt, va_list args);
void c8LogErrorArgs(const char *fmt, va_list args);
void c8LogFatalArgs(const char *fmt, va_list args);

#endif /* LOG_H */
