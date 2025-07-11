#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

#define RK_LOG_IMPLEMENTATION
#include <rklog/rklog.h>

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
    c8LogInfoArgs(fmt, args);
    va_end(args);
}

void c8LogWarning(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    c8LogWarningArgs(fmt, args);
    va_end(args);
}

void c8LogError(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    c8LogErrorArgs(fmt, args);
    va_end(args);
}

void c8LogFatal(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    c8LogFatalArgs(fmt, args);
    va_end(args);
}

void c8LogInfoArgs(const char *fmt, va_list args)
{
    rkLogInfoArgs(s_globalLogger, fmt, args);
}

void c8LogWarningArgs(const char *fmt, va_list args)
{
    rkLogWarningArgs(s_globalLogger, fmt, args);
}

void c8LogErrorArgs(const char *fmt, va_list args)
{
    rkLogErrorArgs(s_globalLogger, fmt, args);
}

void c8LogFatalArgs(const char *fmt, va_list args)
{
    rkLogFatalArgs(s_globalLogger, fmt, args);
}

void c8Panic(Chip8ErrorCode code, const char *fmt, ...)
{
    va_list args = { 0 };

    va_start(args, fmt);
    if (s_globalLogger)
    {
        rkLogFatalArgs(s_globalLogger, fmt, args);
    }
    else
    {
        vfprintf(stderr, fmt, args);
        fputc('\n', stderr);
    }
    va_end(args);

    exit(code);
}

