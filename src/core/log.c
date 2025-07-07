#include "log.h"

#define RK_LOG_IMPLEMENTATION
#include <rklog/rklog.h>

#include <stdarg.h>

static rkLogger *s_globalLogger = NULL;

void c8InitLogging(void)
{
    s_globalLogger = rkDefaultLogger("c8emu");
}

void c8CloseLogging(void)
{
    rkCloseLogger(s_globalLogger);
}

void c8LogInfo(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogInfoArgs(s_globalLogger, fmt, args);
    va_end(args);
}

void c8LogWarning(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogWarningArgs(s_globalLogger, fmt, args);
    va_end(args);
}

void c8LogError(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogErrorArgs(s_globalLogger, fmt, args);
    va_end(args);
}

void c8LogFatal(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogFatalArgs(s_globalLogger, fmt, args);
    va_end(args);
}
