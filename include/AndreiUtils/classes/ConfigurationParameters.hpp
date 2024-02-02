//
// Created by Andrei on 10.11.22.
//

#pragma once

#include <AndreiUtils/classes/Parameters.hpp>

namespace AndreiUtils {
    class ConfigurationParameters : public Parameters {
    public:
        static std::string const parameterForKey;

        explicit ConfigurationParameters(std::string const &fileName, std::string parametersFor);

        explicit ConfigurationParameters(nlohmann::json config, std::string parametersFor);

        ConfigurationParameters() noexcept;

        ConfigurationParameters(ConfigurationParameters const &other);

        ConfigurationParameters(ConfigurationParameters &&other) noexcept;

        ConfigurationParameters &operator=(ConfigurationParameters const &other);

        ConfigurationParameters &operator=(ConfigurationParameters &&other) noexcept;

        bool operator==(ConfigurationParameters const &other) const;

        bool operator!=(ConfigurationParameters const &other) const;

        [[nodiscard]] ConfigurationParameters getSubConfig(std::string const &subParametersFor) const;

    protected:
        bool checkCorrectJsonParameters(nlohmann::json &_config) const;

        std::string parametersFor;
    };
}
