//
// Created by Andrei on 10.11.22.
//

#pragma once

#include <AndreiUtils/json.hpp>
#include <string>

namespace AndreiUtils {
    class Parameters {
    public:
        Parameters();

        explicit Parameters(nlohmann::json config);

        explicit Parameters(nlohmann::json *config);

        Parameters(Parameters const &other);

        Parameters(Parameters &&other) noexcept;

        virtual ~Parameters() noexcept;

        Parameters &operator=(Parameters const &other);

        Parameters &operator=(Parameters &&other) noexcept;

        [[nodiscard]] bool operator==(Parameters const &other) const;

        [[nodiscard]] bool operator!=(Parameters const &other) const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] bool has(std::string const &parameterName) const;

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName) const {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not get json value from a nullptr reference!");
                }
                return this->parameterReference->at(parameterName).get<T>();
            }
            return this->parameters.at(parameterName).get<T>();
        }

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName, T const &defaultValue) const {
            if (this->has(parameterName)) {
                return this->get<T>(parameterName);
            }
            return defaultValue;
        }

        template<typename T>
        void set(std::string const &parameterName, T const &data) {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not set json value in a nullptr reference!");
                }
                (*this->parameterReference)[parameterName] = data;
            } else {
                this->parameters[parameterName] = data;
            }
        }

        template<typename T>
        void set(std::string const &parameterName, T &&data) {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not set json value in a nullptr reference!");
                }
                (*this->parameterReference)[parameterName] = std::forward<T>(data);
            } else {
                this->parameters[parameterName] = std::forward<T>(data);
            }
        }

        virtual bool deleteKey(std::string const &parameterName);

        [[nodiscard]] Parameters operator[](std::string const &parameterName);

        [[nodiscard]] Parameters at(std::string const &parameterName);

        [[nodiscard]] nlohmann::json const &getCreateJson(std::string const &parameterName);

        [[nodiscard]] nlohmann::json const &getJson(std::string const &parameterName) const;

        [[nodiscard]] nlohmann::json const &getJson() const;

    protected:
        [[nodiscard]] nlohmann::json &getCreateJsonReference(std::string const &parameterName);

        [[nodiscard]] nlohmann::json &getJsonReference(std::string const &parameterName);

        [[nodiscard]] nlohmann::json &getJsonReference();

        bool isReference;
        nlohmann::json parameters;
        nlohmann::json *parameterReference;
    };

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

    class ParametersWithExternalConfig : public Parameters {
    public:
        [[nodiscard]] static bool isJsonExternalConfig(nlohmann::json const &j);

        static std::string const externalConfigKey;

        ParametersWithExternalConfig();

        explicit ParametersWithExternalConfig(std::string const &fileName);

        explicit ParametersWithExternalConfig(nlohmann::json config);

        explicit ParametersWithExternalConfig(nlohmann::json *config);

        template<typename T>
        void set(std::string const &parameterName, T const &data) {
            this->Parameters::set<T>(parameterName, data);
            this->processOverwrittenParameter(parameterName);
        }

        template<typename T>
        void set(std::string const &parameterName, T &&data) {
            this->Parameters::set<T>(parameterName, std::forward<T>(data));
            this->processOverwrittenParameter(parameterName);
        }

        bool deleteKey(std::string const &parameterName) override;

        void writeParameters(std::string const &fileName, bool withWriteSubConfigs) const;

        [[nodiscard]] std::string toString(std::string const &indent = "", bool verbose = false) const;

    protected:
        ParametersWithExternalConfig(nlohmann::json config, std::string configFileDirectory);

        ParametersWithExternalConfig(nlohmann::json *config, std::string configFileDirectory);

        void initialize(nlohmann::json *config, bool setReference);

        void processOverwrittenParameter(std::string const &parameterName);

        void updateParameters(nlohmann::json &toWriteParameters, bool withWriteSubConfigs) const;

        void collectAndUpdateParametersToWriteForThisFile(nlohmann::json &parametersToWrite, bool recurse) const;

        std::map<std::string, ParametersWithExternalConfig> externalConfigs;
        bool isExternalConfig;
        std::string externalFileName, originalExternalFileName, configFileDirectory;
    };
}
