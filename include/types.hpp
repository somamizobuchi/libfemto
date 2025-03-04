#pragma once

#include <memory>
#include <expected>
#include "point2.hpp"

namespace femto
{
    /// Pointer type aliases
    // Unique pointer
    template <typename T>
    using uptr_t = std::unique_ptr<T>;

    // Shared pointer
    template <typename T>
    using sptr_t = std::shared_ptr<T>;

    // Weak pointer
    template <typename T>
    using wptr_t = std::weak_ptr<T>;

    // Result type alias
    using result_t = std::expected<void, std::string>;

} // namespace femto