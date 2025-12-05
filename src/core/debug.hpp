#pragma once

#include "platform.hpp"
#include "types.hpp"

#include <cstdlib>
#include <format>

#if defined(C8_DEBUG)
#include <rklog/rklog.hpp>
#else
#include <iostream>
#include <print>
#endif

#if defined(C8_DEBUG)
#define C8_ASSERT(expr, ...) if (!(expr)) ::c8emu::Panic(::c8emu::ErrorCode::ASSERTION_FAILED, __VA_ARGS__)
#else
#define C8_ASSERT(expr, ...) (void)0
#endif

#if defined(C8_DEBUG)
#define C8_INIT_LOGGING()   ::rklog::GlobalLogger::InitConsole("c8emu")
#define C8_LOG_INFO(...)    ::rklog::GlobalLogger::Get()->Info(__VA_ARGS__)
#define C8_LOG_WARNING(...) ::rklog::GlobalLogger::Get()->Warn(__VA_ARGS__)
#define C8_LOG_ERROR(...)   ::rklog::GlobalLogger::Get()->Error(__VA_ARGS__)
#define C8_LOG_FATAL(...)   ::rklog::GlobalLogger::Get()->Fatal(__VA_ARGS__)
#else
#define C8_INIT_LOGGING()   (void)0
#define C8_LOG_INFO(...)    (void)0
#define C8_LOG_WARNING(...) (void)0
#define C8_LOG_ERROR(...)   (void)0
#define C8_LOG_FATAL(...)   (void)0
#endif

namespace c8emu {

enum class ErrorCode : u8
{
    NONE,
    ASSERTION_FAILED,
    FAILED_TO_OPEN_FILE,
    FAILED_TO_READ_ROM,
    OUT_OF_MEMORY,
    FAILED_TO_LOAD_TARGET,
    INVALID_ADDRESS_MODE,
};

template<typename ... Args>
[[noreturn]] void Panic(ErrorCode code, std::format_string<Args...> fmt, Args&& ... args) noexcept
{
#if defined(C8_DEBUG)
    C8_LOG_FATAL(fmt, std::forward<Args>(args)...);
#else
    std::println(std::cerr, fmt, std::forward<Args>(args)...);
#endif
    std::exit(static_cast<int>(code));
}

}
