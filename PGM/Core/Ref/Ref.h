#pragma once

#include <memory>

namespace PGM
{

template <typename T, typename Deleter = std::default_delete<T>> using Ref = std::unique_ptr<T, Deleter>;
template <typename T> using SharedRef = std::shared_ptr<T>;

}