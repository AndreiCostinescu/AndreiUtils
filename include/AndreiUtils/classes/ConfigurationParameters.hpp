//
// Created by Andrei on 10.11.22.
//

#ifndef ANDREIUTILS_CONFIGURATIONPARAMETERS_HPP
#define ANDREIUTILS_CONFIGURATIONPARAMETERS_HPP

#include <AndreiUtils/json.hpp>
#include <string>

namespace AndreiUtils {
    class ConfigurationParameters {
    public:
        explicit ConfigurationParameters(std::string const &fileName, std::string parametersFor);

        explicit ConfigurationParameters(nlohmann::json config, std::string parametersFor);

        ConfigurationParameters() noexcept;

        ConfigurationParameters(ConfigurationParameters const &other);

        ConfigurationParameters(ConfigurationParameters &&other) noexcept;

        ConfigurationParameters &operator=(ConfigurationParameters const &other);

        ConfigurationParameters &operator=(ConfigurationParameters &&other) noexcept;

        bool operator==(ConfigurationParameters const &other) const;

        bool operator!=(ConfigurationParameters const &other) const;

        bool isInitialized() const;

        bool has(std::string const &parameterName) const;

        template<typename T>
        T get(std::string const &parameterName) const {
            if (!this->initialized) {
                throw std::runtime_error("ConfigurationParameters not initialized!");
            }
            return this->config.at(parameterName).get<T>();
        }

        template<typename T>
        T get(std::string const &parameterName, T const &defaultValue) const {
            if (!this->initialized) {
                throw std::runtime_error("ConfigurationParameters not initialized!");
            }
            if (this->has(parameterName)) {
                return this->config.at(parameterName).get<T>();
            }
            return defaultValue;
        }

        ConfigurationParameters getSubConfig(std::string const &subParametersFor) const;

        nlohmann::json const &getJson(std::string const &parameterName) const;

        nlohmann::json &getJson();

        nlohmann::json const &getJson() const;

    protected:
        bool checkCorrectJsonParameters(nlohmann::json &_config) const;

        bool initialized;
        std::string parametersFor;
        nlohmann::json config;
    };
}

#endif //ANDREIUTILS_CONFIGURATIONPARAMETERS_HPP
