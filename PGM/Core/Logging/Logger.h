#pragma once

#include <string_view>

#if PGM_LOGGING_LEVEL > 0
#include <fmt/color.h>
#include <fmt/format.h>
#include <utility>
#endif

namespace PGM::Logging
{

template <typename... Rest>
inline void log_error([[maybe_unused]] std::string_view str, [[maybe_unused]] Rest &&...rest)
{
#if PGM_LOGGING_LEVEL > 0
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red), "[ERROR] {}\n",
               fmt::format(str, std::forward<Rest>(rest)...));
#endif
}

template <typename... Rest>
inline void log_warning([[maybe_unused]] std::string_view str, [[maybe_unused]] Rest &&...rest)
{
#if PGM_LOGGING_LEVEL > 1
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), "[WARNING] {}\n",
               fmt::format(str, std::forward<Rest>(rest)...));
#endif
}

template <typename... Rest> inline void log_info([[maybe_unused]] std::string_view str, [[maybe_unused]] Rest &&...rest)
{
#if PGM_LOGGING_LEVEL > 2
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "[INFO] {}\n",
               fmt::format(str, std::forward<Rest>(rest)...));
#endif
}

template <typename... Rest>
inline void log_debug([[maybe_unused]] std::string_view str, [[maybe_unused]] Rest &&...rest)
{
#if PGM_LOGGING_LEVEL > 3
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::magenta), "[DEBUG] {}\n",
               fmt::format(str, std::forward<Rest>(rest)...));
#endif
}

} // namespace PGM::Logging