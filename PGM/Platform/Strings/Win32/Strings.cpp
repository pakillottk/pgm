#include "../Strings.h"

#include <Windows.h>

std::wstring PGM::Strings::widen_utf8(const std::string_view &utf8)
{
    if (utf8.empty())
    {
        return std::wstring{};
    }
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &utf8[0], (int)utf8.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &utf8[0], (int)utf8.size(), &wstrTo[0], size_needed);
    return wstrTo;
}