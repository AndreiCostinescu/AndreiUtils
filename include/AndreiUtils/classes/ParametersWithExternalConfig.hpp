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

        void writeParameters(std::string const &fileName, bool withWriteSubConfigs, bool keepOrder = false, 
                             bool keepNewLines = true) const;

        [[nodiscard]] std::string toString(std::string const &indent = "", bool verbose = false) const;

    protected:
        ParametersWithExternalConfig(nlohmann::json config, std::string configFileDirectory);

        ParametersWithExternalConfig(nlohmann::json *config, std::string configFileDirectory);

        void initialize(nlohmann::json *config, bool setReference);

        void processOverwrittenParameter(std::string const &parameterName);

        void updateParameters(
                nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const;

        void collectAndUpdateParametersToWriteForThisFile(
                nlohmann::json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const;

        std::map<std::string, ParametersWithExternalConfig> externalConfigs;
        std::map<std::string, ParametersWithExternalConfig> externalShortcuts;
        std::map<std::string, std::vector<std::string>> externalFileToShortcutAssociation;
        std::map<std::string, std::pair<std::string, std::string>> shortcutToExternalFileAssociation;
        std::map<std::string, ParametersWithExternalConfig *> shortcutToParametersAssociation;
        bool isExternalConfig;
        std::string externalFileName, originalExternalFileName, configFileDirectory;
    };
}
