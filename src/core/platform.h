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

#endif /* PLATFORM_H */
