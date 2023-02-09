#include <string_view>

#if PGM_LOGGING_LEVEL > 0
#include <fmt/color.h>
#include <fmt/format.h>
#include <utility>
#endif

namespace PGM { namespace Logging {

template<typename... Rest>
inline void
log_error([[maybe_unused]] const std::string_view& str,
          [[maybe_unused]] Rest&&... rest)
{
#if PGM_LOGGING_LEVEL > 0
  fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::red),
             "[ERROR] {}\n",
             fmt::format(str, std::forward<Rest>(rest)...));
#endif
}

}}