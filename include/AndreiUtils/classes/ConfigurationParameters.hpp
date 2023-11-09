//
// Created by Andrei on 10.11.22.
//

#pragma once

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

        [[nodiscard]] bool empty() const;

        [[nodiscard]] bool has(std::string const &parameterName) const;

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName) const {
            return this->config.at(parameterName).get<T>();
        }

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName, T const &defaultValue) const {
            if (this->has(parameterName)) {
                return this->config.at(parameterName).get<T>();
            }
            return defaultValue;
        }

        template<typename T>
        void set(std::string const &parameterName, T const &data) {
            this->config[parameterName] = data;
        }

        template<typename T>
        void set(std::string const &parameterName, T &&data) {
            this->config[parameterName] = std::forward<T>(data);
        }

        [[nodiscard]] ConfigurationParameters getSubConfig(std::string const &subParametersFor) const;

        [[nodiscard]] nlohmann::json const &getJson(std::string const &parameterName) const;

        [[nodiscard]] nlohmann::json &getJson();

        [[nodiscard]] nlohmann::json const &getJson() const;

    protected:
        bool checkCorrectJsonParameters(nlohmann::json &_config) const;

        std::string parametersFor;
        nlohmann::json config;
    };
}
