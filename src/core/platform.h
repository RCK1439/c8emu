#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
#   define C8_PLATFORM_WINDOWS
#elif defined(__linux__)
#   define C8_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define C8_PLATFORM_APPLE
#else
#   error "Unsupported platform"
#endif

#if defined(DEBUG)
#   define C8_DEBUG
#elif defined(NDEBUG)
#   define C8_RELEASE
#else
#   define C8_DEBUG
#endif

#if defined(__GNUC__)
#   define C8_COMPILER_GCC
#elif defined(__clang__)
#   define C8_COMPILER_CLANG
#elif defined(_MSC_VER)
#   define C8_COMPILER_MSVC
#else
#   error "Unsupported compiler"
#endif

#if defined(C8_PLATFORM_WINDOWS)
#   define SIZE_T_FMT "%llu"
#elif defined(C8_PLATFORM_LINUX) || defined(C8_PLATFORM_APPLE)
#   define SIZE_T_FMT "%lu"
#endif

#if defined(C8_COMPILER_GCC)
#   define UNUSED __attribute__((unused))
#elif defined(C8_COMPILER_CLANG)
#   define UNUSED __attribute__((unused))
#elif defined(C8_COMPILER_MSVC)
#   defined UNUSED [[maybe_unused]]
#endif

#endif /* PLATFORM_H */
