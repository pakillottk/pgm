#pragma once

#include <memory>

namespace PGM
{

template <typename T, typename Deleter = std::default_delete<T>> using Ref = std::unique_ptr<T, Deleter>;

template <typename T, typename Deleter = std::default_delete<T>, typename... Args>
[[nodiscard]] Ref<T, Deleter> make_ref(Args &&...args)
{
    return std::unique_ptr<T, Deleter>(new T(std::forward<Args>(args)...));
}

template <typename T> using SharedRef = std::shared_ptr<T>;

template <typename T, typename... Args> [[nodiscard]] SharedRef<T> make_shared_ref(Args &&...args)
{
    return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

} // namespace PGM