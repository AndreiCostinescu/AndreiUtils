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
        Parameters(), externalDataPtr(nullptr), isExternalDataReference(false), configFileDirectory("./") {}

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
        AndreiUtils::mapDelete(this->getExternalData().externalConfigs, parameterName);
        return true;
    } else {
        return false;
    }
}

ParametersWithExternalConfig ParametersWithExternalConfig::operator[](std::string const &parameterName) {
    ParametersWithExternalConfig *externalConfig;
    if (mapGetIfContains(this->getExternalData().externalKeyToParametersAssociation, parameterName, externalConfig)) {
        return {&this->getCreateJsonReference(parameterName), &externalConfig->getExternalData()};
    }
    if (mapGetIfContains(this->getExternalData().externalConfigs, parameterName, externalConfig)) {
        return {&this->getCreateJsonReference(parameterName), &externalConfig->getExternalData()};
    }
    return ParametersWithExternalConfig(&this->getCreateJsonReference(parameterName));
}

ParametersWithExternalConfig ParametersWithExternalConfig::at(string const &parameterName) {
    ParametersWithExternalConfig *externalConfig;
    if (mapGetIfContains(this->getExternalData().externalKeyToParametersAssociation, parameterName, externalConfig)) {
        return {&this->getJsonReference(parameterName), &externalConfig->getExternalData()};
    }
    if (mapGetIfContains(this->getExternalData().externalConfigs, parameterName, externalConfig)) {
        return {&this->getJsonReference(parameterName), &externalConfig->getExternalData()};
    }
    return {&this->getJsonReference(parameterName), nullptr};
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
    ExternalParameterData const &external = this->getExternalData();
    if (verbose) {
        ss << indent << (this->isReference ? "REFERENCE" : "VALUE") << ": ";
        if (this->isExternalConfig()) {
            ss << "External file " << external.externalFileName << ": ";
        }
        ss << this->configFileDirectory << ": " << endl;
    }
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        map<ParametersWithExternalConfig const *, bool> processedExternalParameters;
        for (auto const &jsonDatum: jsonData.items()) {
            ParametersWithExternalConfig const *p;
            if (AndreiUtils::mapGetIfContains(external.externalConfigs, jsonDatum.key(), p)) {
                ss << indent + AndreiUtils::tab << jsonDatum.key() << ": ";
                ss << "{" << endl << p->toString(indent + AndreiUtils::tab * 2, verbose) << indent + AndreiUtils::tab
                   << "}" << endl;
            } else if (AndreiUtils::mapGetIfContains(external.externalKeyToParametersAssociation, jsonDatum.key(), p)) {
                if (!mapContains(processedExternalParameters, p)) {
                    ss << indent + AndreiUtils::tab << "EXTERNAL: "
                       << AndreiUtils::mapGet(external.externalFileAssociation, p).second << endl;
                    ss << p->toString(indent + AndreiUtils::tab * 2, verbose) << endl;
                    mapEmplace(processedExternalParameters, p, true);
                }
            } else {
                ss << indent + AndreiUtils::tab << jsonDatum.key() << ": ";
                ss << jsonDatum.value().dump(4) << endl;
            }
        }
    } else {
        if (!external.externalConfigs.empty()) {
            ss << jsonData.dump(4) << endl;
            cout << ss.str() << endl;
            assert(external.externalConfigs.empty());
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

ParametersWithExternalConfig::ParametersWithExternalConfig(nlohmann::json config, ExternalParameterData *externalData) :
        Parameters(std::move(config)), externalDataPtr(externalData),
        isExternalDataReference(externalData != nullptr) {}

ParametersWithExternalConfig::ParametersWithExternalConfig(nlohmann::json *config, ExternalParameterData *externalData)
        : Parameters(config), externalDataPtr(externalData), isExternalDataReference(externalData != nullptr) {}

void ParametersWithExternalConfig::initialize(nlohmann::json *config, bool setReference) {  // NOLINT(misc-no-recursion)
    ExternalParameterData &external = this->getExternalData();
    if (config != nullptr && config->contains(ParametersWithExternalConfig::externalConfigKey)) {
        external.isExternalConfig = true;
        external.externalFileName = config->at("externalConfig");
        external.originalExternalFileName = external.externalFileName;
        if (!isFilePathAbsolute(external.externalFileName)) {
            external.externalFileName = this->configFileDirectory + external.externalFileName;
            external.externalFileName = simplifyRelativePath(external.externalFileName);
        }
        auto tmp = ParametersWithExternalConfig(external.externalFileName);
        this->parameters = std::move(tmp.parameters);
        ExternalParameterData &tmpExternal = tmp.getExternalData();
        external.externalConfigs = std::move(tmpExternal.externalConfigs);
        external.externalParameters = std::move(tmpExternal.externalParameters);
        external.externalKeyToParametersAssociation = std::move(tmpExternal.externalKeyToParametersAssociation);
        external.externalFileAssociation = std::move(tmpExternal.externalFileAssociation);
        external.externalFileToExternalKeyAssociation = std::move(tmpExternal.externalFileToExternalKeyAssociation);
        if (tmp.isExternalConfig()) {
            external.externalFileName = std::move(tmpExternal.externalFileName);
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
                if (subConfig.isExternalConfig() || !subConfig.getExternalData().externalConfigs.empty()) {
                    assert(subConfig.isReference);
                    AndreiUtils::mapEmplace(external.externalConfigs, jsonData.key(), std::move(subConfig));
                } else if (jsonData.key() == ParametersWithExternalConfig::externalDataKey) {
                    hasExternalConfigs = true;
                    if (!jsonData.value().is_array()) {
                        throw std::runtime_error("\"" + ParametersWithExternalConfig::externalDataKey +
                                                 "\" key may be used wrong in data! Not expected non-array type!");
                    }
                }
            }
            if (hasExternalConfigs) {
                auto externalFiles = jsonConfig.at(
                        ParametersWithExternalConfig::externalDataKey).get<std::vector<std::string>>();
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
                    mapEmplace(external.externalFileToExternalKeyAssociation, originalExternalFile);
                    auto *parameterPtr = &(mapEmplace(external.externalParameters, originalExternalFile,
                                                      std::move(tmp))->second);
                    mapEmplace(external.externalFileAssociation, parameterPtr, originalExternalFile, externalFile);
                    for (auto const &constJsonData: parameterPtr->getJson().items()) {
                        std::string const &key = constJsonData.key();
                        mapGet(external.externalFileToExternalKeyAssociation, originalExternalFile).emplace_back(key);
                        mapEmplace(external.externalKeyToParametersAssociation, key, parameterPtr);
                    }
                }
                jsonConfig.erase(ParametersWithExternalConfig::externalDataKey);
            }
        }
    }
}

void ParametersWithExternalConfig::processOverwrittenParameter(  // NOLINT(misc-no-recursion)
        std::string const &parameterName) {
    ParametersWithExternalConfig *overwrittenConfig;
    ExternalParameterData &external = this->getExternalData();
    if (AndreiUtils::mapGetIfContains(external.externalConfigs, parameterName, overwrittenConfig)) {
        ParametersWithExternalConfig subConfig(&this->getJsonReference(parameterName), this->configFileDirectory);
        if (subConfig.isExternalConfig() || !subConfig.getExternalData().externalConfigs.empty()) {
            *overwrittenConfig = std::move(subConfig);
        }
    }
    if (AndreiUtils::mapGetIfContains(external.externalKeyToParametersAssociation, parameterName, overwrittenConfig)) {
        // iterate recursively until the parameterName is no longer an external member!
        while (AndreiUtils::mapGetIfContains(overwrittenConfig->getExternalData().externalKeyToParametersAssociation,
                                             parameterName, overwrittenConfig)) {}
        overwrittenConfig->set(parameterName, this->getJson(parameterName));
    }
}

void ParametersWithExternalConfig::updateParameters(  // NOLINT(misc-no-recursion)
        nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const {
    nlohmann::json theseParameters;
    this->collectAndUpdateParametersToWriteForThisFile(theseParameters, recurseSubConfigs, keepOrder, keepNewLines);
    ExternalParameterData const &external = this->getExternalData();
    if (!external.externalParameters.empty()) {
        for (auto const &externalParameters: external.externalParameters) {
            auto processedExternalFile = mapGet(external.externalFileAssociation, &externalParameters.second).second;
            nlohmann::json toWriteExternalParameters;
            externalParameters.second.updateParameters(toWriteExternalParameters, recurseSubConfigs, keepOrder, keepNewLines);
            writeJsonFile(processedExternalFile, toWriteExternalParameters, keepOrder, keepNewLines);
            /*
            externalParameters.second.writeParameters(processedExternalFile, recurseSubConfigs, keepOrder,
                                                      keepNewLines);
            //*/
        }
        theseParameters[ParametersWithExternalConfig::externalDataKey] = getMapKeys(external.externalParameters);
    }
    if (this->isExternalConfig()) {
        toWriteParameters[ParametersWithExternalConfig::externalConfigKey] = external.originalExternalFileName;
        // cout << "Writing " << theseParameters.dump(4) << " to file " << this->externalFileName << endl;
        writeJsonFile(external.externalFileName, theseParameters, keepOrder, keepNewLines);
    } else {
        toWriteParameters = std::move(theseParameters);
    }
}

void ParametersWithExternalConfig::collectAndUpdateParametersToWriteForThisFile(  // NOLINT(misc-no-recursion)
        json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) const {
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        ExternalParameterData const &external = this->getExternalData();
        for (auto const &datum: jsonData.items()) {
            ParametersWithExternalConfig const *subConfig;
            if (AndreiUtils::mapGetIfContains(external.externalConfigs, datum.key(), subConfig)) {
                json subConfigJson;
                subConfig->updateParameters(subConfigJson, recurseSubConfigs, keepOrder, keepNewLines);
                parametersToWrite[datum.key()] = subConfigJson;
            } else if (!AndreiUtils::mapContains(external.externalKeyToParametersAssociation, datum.key())) {
                parametersToWrite[datum.key()] = datum.value();
            }
        }
    } else {
        parametersToWrite = jsonData;
    }
}

bool ParametersWithExternalConfig::isExternalConfig() const {
    if (this->isExternalDataReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not use data from a nullptr external data reference!");
        }
        return this->externalDataPtr->isExternalConfig;
    }
    return this->externalData.isExternalConfig;
}

ParametersWithExternalConfig::ExternalParameterData const &ParametersWithExternalConfig::getExternalData() const {
    if (this->isExternalDataReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not use data from a nullptr external data reference!");
        }
        return *this->externalDataPtr;
    }
    return this->externalData;
}

ParametersWithExternalConfig::ExternalParameterData &ParametersWithExternalConfig::getExternalData() {
    if (this->isExternalDataReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not use data from a nullptr external data reference!");
        }
        return *this->externalDataPtr;
    }
    return this->externalData;
}

ParametersWithExternalConfig::ExternalParameterData::ExternalParameterData() : isExternalConfig(false) {}
