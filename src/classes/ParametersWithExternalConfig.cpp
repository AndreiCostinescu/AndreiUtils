//
// Created by Andrei on 18.12.23.
//

#include <AndreiUtils/classes/ParametersWithExternalConfig.hpp>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsString.h>
#include <iostream>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

using json = nlohmann::json;

bool ParametersWithExternalConfig::isJsonExternalConfig(json const &j) {
    return j.is_object() && j.contains(ParametersWithExternalConfig::externalConfigKey);
}

std::string const ParametersWithExternalConfig::externalConfigKey("externalConfig");

ParametersWithExternalConfig::ParametersWithExternalConfig() :
        Parameters(), isExternalConfig(false), configFileDirectory("./") {}

ParametersWithExternalConfig::ParametersWithExternalConfig(string const &fileName) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->configFileDirectory = getRelativeDirectoryOfPath(fileName);
    auto tmpJson = readJsonFile(fileName);
    this->initialize(&tmpJson, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(nlohmann::json config) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->initialize(&config, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(nlohmann::json *config) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->initialize(config, true);
}

void ParametersWithExternalConfig::set(nlohmann::json const &data) {
    this->Parameters::set(data);
    // TODO: process this->externalConfigs
}

void ParametersWithExternalConfig::set(nlohmann::json &&data) {
    this->Parameters::set(std::forward<nlohmann::json>(data));
    // TODO: process this->externalConfigs
}

bool ParametersWithExternalConfig::deleteKey(std::string const &parameterName) {
    if (this->Parameters::deleteKey(parameterName)) {
        // bool res = AndreiUtils::mapDelete(this->externalConfigs, parameterName); assert(res);
        AndreiUtils::mapDelete(this->externalConfigs, parameterName);
        return true;
    } else {
        return false;
    }
}

void ParametersWithExternalConfig::writeParameters(std::string const &fileName, bool withWriteSubConfigs,
                                                   bool keepOrder, bool keepNewLines) const {
    nlohmann::json toWriteParameters;
    this->updateParameters(toWriteParameters, withWriteSubConfigs, keepOrder, keepNewLines);
    // cout << "Writing " << toWriteParameters.dump(4) << " to file " << fileName << endl;
    writeJsonFile(fileName, toWriteParameters, keepOrder, keepNewLines);
}

std::string ParametersWithExternalConfig::toString(  // NOLINT(misc-no-recursion)
        string const &indent, bool verbose) const {
    stringstream ss;
    if (verbose) {
        ss << indent << (this->isReference ? "REFERENCE" : "VALUE") << ": ";
        if (this->isExternalConfig) {
            ss << "External file " << this->externalFileName << ": ";
        }
        ss << this->configFileDirectory << ": " << endl;
    }
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        for (auto const &jsonDatum: jsonData.items()) {
            ss << indent + AndreiUtils::tab << jsonDatum.key() << ": ";
            ParametersWithExternalConfig const *p;
            if (AndreiUtils::mapGetIfContains(this->externalConfigs, jsonDatum.key(), p)) {
                ss << "{" << endl << p->toString(indent + AndreiUtils::tab * 2, verbose) << indent + AndreiUtils::tab
                   << "}" << endl;
            } else {
                ss << jsonDatum.value().dump(4) << endl;
            }
        }
    } else {
        if (!this->externalConfigs.empty()) {
            ss << jsonData.dump(4) << endl;
            cout << ss.str() << endl;
            assert(this->externalConfigs.empty());
        }
        ss << indent + AndreiUtils::tab << jsonData.dump(4) << endl;
    }
    return ss.str();
}

ParametersWithExternalConfig::ParametersWithExternalConfig(nlohmann::json config, std::string configFileDirectory) :
        ParametersWithExternalConfig() {
    this->configFileDirectory = std::move(configFileDirectory);
    this->initialize(&config, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(  // NOLINT(misc-no-recursion)
        nlohmann::json *config, std::string configFileDirectory) : ParametersWithExternalConfig() {
    this->configFileDirectory = std::move(configFileDirectory);
    this->initialize(config, true);
}

void ParametersWithExternalConfig::initialize(nlohmann::json *config, bool setReference) {  // NOLINT(misc-no-recursion)
    if (config != nullptr && config->contains(ParametersWithExternalConfig::externalConfigKey)) {
        this->isExternalConfig = true;
        this->externalFileName = config->at("externalConfig");
        this->originalExternalFileName = this->externalFileName;
        if (!isFilePathAbsolute(this->externalFileName)) {
            this->externalFileName = this->configFileDirectory + this->externalFileName;
            this->externalFileName = simplifyRelativePath(this->externalFileName);
        }
        auto tmp = ParametersWithExternalConfig(this->externalFileName);
        this->parameters = std::move(tmp.parameters);
        this->externalConfigs = std::move(tmp.externalConfigs);
        if (tmp.isExternalConfig) {
            this->externalFileName = std::move(tmp.externalFileName);
        }

        if (setReference) {
            *config = std::move(this->parameters);  // important to move the data to keep the pointer-references intact!
            this->parameterReference = config;
            this->isReference = true;
        }
    } else {
        if (setReference) {
            this->parameterReference = config;
            this->isReference = (this->parameterReference != nullptr);
        } else {
            this->parameters = std::move(*config);
        }

        auto &jsonConfig = this->getJsonReference();
        if (jsonConfig.is_object()) {
            // create map of external configs
            for (auto &jsonData: jsonConfig.items()) {
                auto &jsonValue = jsonData.value();
                ParametersWithExternalConfig subConfig(&jsonValue, this->configFileDirectory);
                if (subConfig.isExternalConfig || !subConfig.externalConfigs.empty()) {
                    assert(subConfig.isReference);
                    AndreiUtils::mapEmplace(this->externalConfigs, jsonData.key(), std::move(subConfig));
                }
            }
        }
    }
}

void ParametersWithExternalConfig::processOverwrittenParameter(std::string const &parameterName) {
    ParametersWithExternalConfig *overwrittenConfig;
    if (AndreiUtils::mapGetIfContains(this->externalConfigs, parameterName, overwrittenConfig)) {
        ParametersWithExternalConfig subConfig(&this->getJsonReference(parameterName), this->configFileDirectory);
        if (subConfig.isExternalConfig || !subConfig.externalConfigs.empty()) {
            *overwrittenConfig = std::move(subConfig);
        }
    }
}

void ParametersWithExternalConfig::updateParameters(  // NOLINT(misc-no-recursion)
        nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const {
    nlohmann::json theseParameters;
    this->collectAndUpdateParametersToWriteForThisFile(theseParameters, recurseSubConfigs, keepOrder, keepNewLines);
    if (this->isExternalConfig) {
        toWriteParameters[ParametersWithExternalConfig::externalConfigKey] = this->originalExternalFileName;
        // cout << "Writing " << theseParameters.dump(4) << " to file " << this->externalFileName << endl;
        writeJsonFile(this->externalFileName, theseParameters, keepOrder, keepNewLines);
    } else {
        toWriteParameters = std::move(theseParameters);
    }
}

void ParametersWithExternalConfig::collectAndUpdateParametersToWriteForThisFile(  // NOLINT(misc-no-recursion)
        json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const {
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        for (auto const &datum: jsonData.items()) {
            ParametersWithExternalConfig const *subConfig;
            if (AndreiUtils::mapGetIfContains(this->externalConfigs, datum.key(), subConfig)) {
                json subConfigJson;
                subConfig->updateParameters(subConfigJson, recurseSubConfigs, keepOrder, keepNewLines);
                parametersToWrite[datum.key()] = subConfigJson;
            } else {
                parametersToWrite[datum.key()] = datum.value();
            }
        }
    } else {
        parametersToWrite = jsonData;
    }
}
