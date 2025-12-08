#pragma once

#include "platform.hpp"

#include <format>
#include <cstdint>
#include <chrono>

namespace rklog {

/**
 * Class representing the time stamp according to the system clock
 */
class TimeStamp final
{
public:
    TimeStamp() = default;

    /**
     * Gets the current time of the system clock
     *
     * @return
     *      The current system time stamp
     */
    static TimeStamp Now() noexcept
    {
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        const std::time_t cNow = std::chrono::system_clock::to_time_t(now);

#if defined(RKLOG_PLATFORM_WINDOWS)
        std::tm localTime;
        const errno_t err = localtime_s(&localTime, &cNow);
        if (err == 0)
        {
            return TimeStamp(static_cast<uint32_t>(localTime.tm_hour), static_cast<uint32_t>(localTime.tm_min), static_cast<uint32_t>(localTime.tm_sec));
        }

        return TimeStamp();
#else
    const std::tm* const localTime = std::localtime(&cNow);
    return TimeStamp(static_cast<uint32_t>(localTime->tm_hour), static_cast<uint32_t>(localTime->tm_min), static_cast<uint32_t>(localTime->tm_sec));
#endif
    }

    /**
     * Gets the hours of the time stamp
     *
     * @return
     *      The number of hours
     */
    inline uint32_t Hours() const noexcept
    {
        return m_Hours;
    }

    /**
     * Gets the minutes of the time stamp
     *
     * @return
     *      The number of minutes
     */
    inline uint32_t Minutes() const noexcept
    {
        return m_Minutes;
    }

    /**
     * Gets the seconds of the time stamp
     *
     * @return
     *      The number of seconds
     */
    inline uint32_t Seconds() const noexcept
    {
        return m_Seconds;
    }

private:
    /**
     * Creates a new instance of the `TimeStamp`
     *
     * @param hours
     *      The number of hours
     * @param minutes
     *      The number of minutes
     * @param seconds
     *      The number of seconds
     */
    constexpr explicit TimeStamp(uint32_t hours, uint32_t minutes, uint32_t seconds) noexcept :
        m_Hours(hours), m_Minutes(minutes), m_Seconds(seconds) {}

private:
    const uint32_t m_Hours = 0;   // The hours of the time stamp
    const uint32_t m_Minutes = 0; // The minutes of the time stamp
    const uint32_t m_Seconds = 0; // The seconds of the time stamp
};

}

// --- custom formatting implementation of the time stamp ---------------------

template<>
struct std::formatter<rklog::TimeStamp>
{
    constexpr auto parse(std::format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    auto format(const rklog::TimeStamp& timeStamp, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "{:02}:{:02}:{:02}", timeStamp.Hours(), timeStamp.Minutes(), timeStamp.Seconds());
    }
};
