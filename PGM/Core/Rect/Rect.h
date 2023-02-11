#pragma once

#include <type_traits>

namespace PGM
{

template <typename T> struct __Rect__
{
    static_assert(std::is_arithmetic_v<T>, "T must be numeric type");

    T x;
    T y;
    T width;
    T height;
};

using Rect = __Rect__<float>;
using RectInt = __Rect__<int>;
using RectUint = __Rect__<unsigned>;
using RectDbl = __Rect__<double>;

} // namespace PGM