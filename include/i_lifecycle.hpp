#pragma once
#include "types.hpp"

namespace femto
{

    /// @brief Enumeration of possible lifecycle states
    enum class LifecycleState
    {
        UNINITIALIZED,
        INITIALIZED,
        PAUSED,
        RESUME,
        RUNNING,
        SHUTDOWN
    };

    /// @brief Interface for managing the life cycle of an object
    class ILifecycle
    {
    public:
        /// @brief Destructor
        virtual ~ILifecycle() = default;

        /// @brief Initialize resources
        /// @result the result of the operation
        virtual result_t initialize() = 0;

        /// @brief Pause execution
        /// @return the result of the operation
        virtual result_t pause() = 0;

        /// @brief Resume execution
        /// @return the result of the operation
        virtual result_t resume() = 0;

        /// @brief Cleanup resources
        /// @return the result of the operation
        virtual result_t shutdown() = 0;
    };
}