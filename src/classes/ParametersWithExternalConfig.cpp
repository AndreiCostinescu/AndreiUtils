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
std::string const ParametersWithExternalConfig::externalDataKey("external");

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
            } else if (AndreiUtils::mapContains(this->shortcutToParametersAssociation, jsonDatum.key())) {
                if (!AndreiUtils::mapContains(this->shortcutToExternalFileAssociation, jsonDatum.key())) {
                    ss << "LAYERED EXTERNAL: " << jsonDatum.value().dump(4) << endl;
                } else {
                    ss << "EXTERNAL: " << jsonDatum.value().dump(4) << endl;
                }
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
        this->shortcutToParametersAssociation = std::move(tmp.shortcutToParametersAssociation);
        this->shortcutToExternalFileAssociation = std::move(tmp.shortcutToExternalFileAssociation);
        this->externalFileToShortcutAssociation = std::move(tmp.externalFileToShortcutAssociation);
        this->externalShortcuts = std::move(tmp.externalShortcuts);
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
            bool hasExternalConfigs = false;
            for (auto &jsonData: jsonConfig.items()) {
                auto &jsonValue = jsonData.value();
                ParametersWithExternalConfig subConfig(&jsonValue, this->configFileDirectory);
                // if the subconfig is external or it has inside its data a nested external config somewhere
                if (subConfig.isExternalConfig || !subConfig.externalConfigs.empty()) {
                    assert(subConfig.isReference);
                    AndreiUtils::mapEmplace(this->externalConfigs, jsonData.key(), std::move(subConfig));
                } else if (jsonData.key() == ParametersWithExternalConfig::externalDataKey) {
                    hasExternalConfigs = true;
                    if (!jsonData.value().is_array()) {
                        throw std::runtime_error("\"" + ParametersWithExternalConfig::externalDataKey +
                                                 "\" key may be used wrong in data! Not expected non-array type!");
                    }
                }
            }
            if (hasExternalConfigs) {
                auto externalFiles = jsonConfig.at(ParametersWithExternalConfig::externalDataKey).get<std::vector<std::string>>();
                for (auto externalFile: externalFiles) {
                    cout << "Found external file: " << externalFile << endl;
                    string originalExternalFile = externalFile;
                    if (!isFilePathAbsolute(externalFile)) {
                        externalFile = simplifyRelativePath(this->configFileDirectory + externalFile);
                    }

                    auto tmp = ParametersWithExternalConfig(externalFile);
                    // assert that keys in tmp are not in jsonConfig (i.e. not in this level!)
                    for (auto const &constJsonData: jsonConfig.items()) {
                        auto const &p = constJsonData.key();
                        if (tmp.has(p)) {
                            throw std::runtime_error(
                                    "External config \"" + externalFile + "\" has the same parameter \"" + p +
                                    "\" as this level! Bad configuration!");
                        }
                    }

                    jsonConfig.update(tmp.getJson());
                    mapEmplace(this->externalFileToShortcutAssociation, originalExternalFile);
                    auto *parameterPtr = &(mapEmplace(this->externalShortcuts, originalExternalFile, std::move(tmp))->second);
                    for (auto const &constJsonData: parameterPtr->getJson().items()) {
                        std::string const &key = constJsonData.key();
                        mapGet(this->externalFileToShortcutAssociation, originalExternalFile).emplace_back(key);
                        mapEmplace(this->shortcutToParametersAssociation, constJsonData.key(), parameterPtr);
                        if (!mapContains(parameterPtr->shortcutToParametersAssociation, key)) {
                            mapEmplace(this->shortcutToExternalFileAssociation, key, originalExternalFile, externalFile);
                        }
                    }
                }
                jsonConfig.erase(ParametersWithExternalConfig::externalDataKey);
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
