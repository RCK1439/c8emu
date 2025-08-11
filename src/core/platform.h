#ifndef PLATFORM_H
#define PLATFORM_H

// --- operating system detection ---------------------------------------------

#if defined(_WIN32) || defined(_WIN64)
#   define C8_PLATFORM_WINDOWS
#elif defined(__linux__)
#   define C8_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define C8_PLATFORM_APPLE
#else
#   error "Unsupported platform"
#endif

// --- build mode configuration -----------------------------------------------

#if defined(DEBUG)
#   define C8_DEBUG
#elif defined(NDEBUG)
#   define C8_RELEASE
#else
#   define C8_DEBUG
#endif

// --- compiler detection -----------------------------------------------------

#if defined(__GNUC__)
#   define C8_COMPILER_GCC
#elif defined(__clang__)
#   define C8_COMPILER_CLANG
#elif defined(_MSC_VER)
#   define C8_COMPILER_MSVC
#else
#   error "Unsupported compiler"
#endif

// --- compiler-specific attributes -------------------------------------------

#if defined(C8_COMPILER_GCC)
#   define UNUSED __attribute__((unused))
#elif defined(C8_COMPILER_CLANG)
#   define UNUSED __attribute__((unused))
#elif defined(C8_COMPILER_MSVC)
#   defined UNUSED [[maybe_unused]]
#endif

#endif /* PLATFORM_H */
