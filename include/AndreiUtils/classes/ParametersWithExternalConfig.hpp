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
            std::map<ParametersWithExternalConfig *, std::pair<std::string, std::string>> externalFileAssociation;
            std::map<ParametersWithExternalConfig *, std::map<std::string, bool>> externalParameterKeyAssociation;
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

        static void toStringMap(
                std::stringstream &ss, std::string const &key, nlohmann::json const &value, std::string const &indent,
                std::map<ParametersWithExternalConfig const *, bool> &processedExternalParameters,
                ExternalParameterData const &external, bool verbose);

        [[nodiscard]] std::string toStringPrivate(
                std::map<std::string, bool> const &collectOnlyTheseKeys = {}, std::string const &indent = "",
                bool verbose = false) const;

        void updateParameters(
                nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines,
                std::map<std::string, bool> const &collectOnlyTheseParameters = {}) const;

        static void collectAndUpdateParametersToWriteForThisFileObjectData(
                std::string const &datumKey, nlohmann::json const &datumValue, ExternalParameterData const &external,
                nlohmann::json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines);

        void collectAndUpdateParametersToWriteForThisFile(
                nlohmann::json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines,
                std::map<std::string, bool> const &collectOnlyTheseKeys) const;

        void updateExternalParameters(nlohmann::json *parameterParent);

        [[nodiscard]] bool isExternalConfig() const;

        [[nodiscard]] ExternalParameterData const &getExternalData() const;

        [[nodiscard]] ExternalParameterData &getExternalData();

        std::string configFileDirectory;
        ExternalParameterData externalData, *externalDataPtr;
        bool isExternalDataReference;
    };
}
