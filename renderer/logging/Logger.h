#if PGM_LOGGING_LEVEL > 0
#include <iostream>
#include <utility>
#endif

namespace PGM { namespace Logging {

    void log_error([[maybe_unused]] const auto& data)
    {
        #if PGM_LOGGING_LEVEL > 0
        std::cout << data << '\n';
        #endif
    }

    void log_error([[maybe_unused]] const auto& data, [[maybe_unused]] auto&&... rest)
    {
        #if PGM_LOGGING_LEVEL > 0
        std::cout << data;
        log_error(std::forward<decltype(rest)>(rest)...);
        #endif
    }

}}