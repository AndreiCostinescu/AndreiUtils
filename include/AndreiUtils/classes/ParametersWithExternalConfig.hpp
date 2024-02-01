//
// Created by Andrei on 18.12.23.
//

#pragma once

#include <AndreiUtils/classes/Parameters.hpp>

namespace AndreiUtils {
    class ParametersWithExternalConfig : public Parameters {
    public:
        [[nodiscard]] static bool isJsonExternalConfig(nlohmann::json const &j);

        static std::string const externalConfigKey, externalDataKey;

        ParametersWithExternalConfig();

        explicit ParametersWithExternalConfig(std::string const &fileName);

        explicit ParametersWithExternalConfig(nlohmann::json config);

        explicit ParametersWithExternalConfig(nlohmann::json *config);

        void set(nlohmann::json const &data) override;

        void set(nlohmann::json &&data) override;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
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
#pragma clang diagnostic pop

        bool deleteKey(std::string const &parameterName) override;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
        [[nodiscard]] ParametersWithExternalConfig operator[](std::string const &parameterName);

        [[nodiscard]] ParametersWithExternalConfig at(std::string const &parameterName);
#pragma clang diagnostic pop

        void writeParameters(std::string const &fileName, bool withWriteSubConfigs, bool keepOrder = false, 
                             bool keepNewLines = true) const;

        [[nodiscard]] std::string toString(std::string const &indent = "", bool verbose = false) const;

    protected:
        class ExternalParameterData {
        public:
            ExternalParameterData();

            std::map<std::string, ParametersWithExternalConfig> externalConfigs;
            std::map<std::string, ParametersWithExternalConfig> externalParameters;
            std::map<std::string, std::vector<std::string>> externalFileToExternalKeyAssociation;
            std::map<ParametersWithExternalConfig *, std::pair<std::string, std::string>> externalFileAssociation;
            std::map<std::string, ParametersWithExternalConfig *> externalKeyToParametersAssociation;
            bool isExternalConfig;
            std::string externalFileName, originalExternalFileName;
        };

        ParametersWithExternalConfig(nlohmann::json config, std::string configFileDirectory);

        ParametersWithExternalConfig(nlohmann::json *config, std::string configFileDirectory);

        ParametersWithExternalConfig(nlohmann::json config, ExternalParameterData *externalData);

        ParametersWithExternalConfig(nlohmann::json *config, ExternalParameterData *externalData);

        void initialize(nlohmann::json *config, bool setReference);

        void processOverwrittenParameter(std::string const &parameterName);

        void updateParameters(
                nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const;

        void collectAndUpdateParametersToWriteForThisFile(
                nlohmann::json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const;

        [[nodiscard]] bool isExternalConfig() const;

        [[nodiscard]] ExternalParameterData const &getExternalData() const;

        [[nodiscard]] ExternalParameterData &getExternalData();

        std::string configFileDirectory;
        ExternalParameterData externalData, *externalDataPtr;
        bool isExternalDataReference;
    };
}
