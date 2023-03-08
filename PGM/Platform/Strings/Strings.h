#pragma once

#include <string>
#include <string_view>

namespace PGM::Strings
{

std::wstring widen_utf8(std::string_view utf8);

}