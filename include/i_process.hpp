#pragma once
#include "i_lifecycle.hpp"
#include "i_configurable.hpp"

namespace femto
{
    /// @brief Interface for a process of execution
    /// @tparam TConfig the configuration type
    template <typename TConfig, typename TInput, typename TOutput>
    class IProcess : public ILifecycle, public IConfigurable<TConfig>
    {
    public:
        IProcess() = default;

        /// @brief Execute the process
        /// @param input the input data
        /// @return the output data
        virtual std::expected<TOutput, std::string> execute(const TInput &input) = 0;

        virtual ~IProcess() = 0;
    };

} // namespace femto
