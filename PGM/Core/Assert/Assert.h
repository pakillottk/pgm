#pragma once

#include <fmt/color.h>
#include <fmt/format.h>

namespace PGM::Assert::detail
{

inline void assert_impl(const char *expr_str, const char *file, int line)
{
    fmt::print("\n");
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red), "Assert failed!\n");
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red), "--------------\n");
    fmt::print(fmt::fg(fmt::color::red), "Check:\t{}\n", expr_str);
    fmt::print(fmt::fg(fmt::color::red), "Source:\t{}:{}\n", file, line);
}

inline void assert_msg_begin()
{
    fmt::print(fmt::fg(fmt::color::red), "Detail:\t");
}

template <typename Arg, typename... Args> inline void assert_msg_impl(Arg &&arg, Args &&...args)
{
    fmt::print(fmt::fg(fmt::color::red), "{}", std::forward<Arg>(arg));
    assert_msg_impl(std::forward<Args>(args)...);
}

inline void assert_msg_impl()
{
    fmt::print("\n\n");
}

} // namespace PGM::Assert::detail

#ifdef _DEBUG
#ifndef _WIN32
#include <signal.h>
#define PGM_DBG_BRK() raise(SIGTRAP)
#else
#define PGM_DBG_BRK() __debugbreak()
#endif
#else
#define PGM_DBG_BRK()
#endif

#ifdef PGM_ASSERTS_ENABLED
#define PGM_ASSERT(Expr, ...)                                                                                          \
    if (!(Expr))                                                                                                       \
    {                                                                                                                  \
        ::PGM::Assert::detail::assert_impl(#Expr, __FILE__, __LINE__);                                                 \
        ::PGM::Assert::detail::assert_msg_begin();                                                                     \
        ::PGM::Assert::detail::assert_msg_impl(__VA_ARGS__);                                                           \
        PGM_DBG_BRK();                                                                                                 \
    }
#else
#define PGM_ASSERT(Expr, ...)
#endif