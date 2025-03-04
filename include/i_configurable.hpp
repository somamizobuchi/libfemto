#pragma once

namespace femto
{
    template <typename T>
    class IConfigurable
    {
    public:
        IConfigurable() = default;
        virtual ~IConfigurable() = default;

        /// @brief Configure object
        /// @param config the new configuration
        virtual result_t configure(const T &config) = 0;

        virtual T get_configuration() const = 0;
    };
}