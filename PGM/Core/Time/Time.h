#pragma once

#include <chrono>

namespace PGM
{

using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;

class Timespan
{
  public:
    constexpr Timespan(const Time &start, const Time &end) : m_Elapsed(end - start)
    {
    }

    constexpr float nanoseconds() const
    {
        return m_Elapsed.count();
    }

    constexpr float microseconds() const
    {
        return nanoseconds() / 1000;
    }

    constexpr float milliseconds() const
    {
        return microseconds() / 1000;
    }

    constexpr float seconds() const
    {
        return milliseconds() / 1000;
    }

  private:
    std::chrono::duration<float, std::nano> m_Elapsed;
};

class Timer
{
  public:
    inline Timer()
    {
        reset();
    }

    inline void reset()
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    static inline Time now()
    {
        return std::chrono::high_resolution_clock::now();
    }

    inline Timespan elapsed() const
    {
        return Timespan{m_Start, std::chrono::high_resolution_clock::now()};
    }

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
};

} // namespace PGM