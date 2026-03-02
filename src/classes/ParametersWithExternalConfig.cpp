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
        Parameters(), externalDataPtr(nullptr), isExternalDataReference(false), rootDirectoryForExternalData("./"),
        externalFilesRelativeToFileLocation(true) {}

ParametersWithExternalConfig::ParametersWithExternalConfig(
        string const &fileName, std::string const &rootDirectoryForExternalData) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->externalFilesRelativeToFileLocation = rootDirectoryForExternalData.empty();
    if (this->externalFilesRelativeToFileLocation) {
        this->rootDirectoryForExternalData = AndreiUtils::getRelativeDirectoryOfPath(fileName);
    } else {
        this->rootDirectoryForExternalData = rootDirectoryForExternalData;
    }
    auto tmpJson = readJsonFile(fileName);
    this->initialize(&tmpJson, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(
        nlohmann::json config, std::string const &rootDirectoryForExternalData) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->externalFilesRelativeToFileLocation = rootDirectoryForExternalData.empty();
    if (!this->externalFilesRelativeToFileLocation) {
        this->rootDirectoryForExternalData = rootDirectoryForExternalData;
    }
    this->initialize(&config, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(
        nlohmann::json *config, std::string const &rootDirectoryForExternalData) :  // NOLINT(misc-no-recursion)
        ParametersWithExternalConfig() {
    this->externalFilesRelativeToFileLocation = rootDirectoryForExternalData.empty();
    if (!this->externalFilesRelativeToFileLocation) {
        this->rootDirectoryForExternalData = rootDirectoryForExternalData;
    }
    this->initialize(config, true);
}

void ParametersWithExternalConfig::set(nlohmann::json const &data) {
    this->Parameters::set(data);
    // TODO: process this->externalConfigs
}

void ParametersWithExternalConfig::set(nlohmann::json &&data) {
    this->Parameters::set(std::forward<nlohmann::json>(data));
    // TODO: process this->externalConfigs
    // TODO: process this->externalParameters
}

bool ParametersWithExternalConfig::deleteKey(std::string const &parameterName) {
    if (this->Parameters::deleteKey(parameterName)) {
        // bool res = AndreiUtils::mapDelete(this->externalConfigs, parameterName); assert(res);
        ExternalParameterData *external = &this->getExternalData();
        AndreiUtils::mapDelete(external->externalConfigs, parameterName);
        // process external keys
        if (AndreiUtils::mapContains(external->externalKeyToParametersAssociation, parameterName)) {
            ParametersWithExternalConfig *p;
            while (AndreiUtils::mapGetIfContains(external->externalKeyToParametersAssociation, parameterName, p)) {
                AndreiUtils::mapDelete(external->externalKeyToParametersAssociation, parameterName);
                AndreiUtils::mapDelete(mapGet(external->externalParameterKeyAssociation, p), parameterName);
                external = &p->getExternalData();
            }
        }
        return true;
    } else {
        return false;
    }
}

ParametersWithExternalConfig ParametersWithExternalConfig::operator[](std::string const &parameterName) {
    ParametersWithExternalConfig *externalConfig;
    if (mapGetIfContains(this->getExternalData().externalKeyToParametersAssociation, parameterName, externalConfig)) {
        return {&this->getCreateJsonReference(parameterName), &externalConfig->getExternalData(), this->getRootDirectoryForExternalDataForConstructorArgument()};
    }
    if (mapGetIfContains(this->getExternalData().externalConfigs, parameterName, externalConfig)) {
        return {&this->getCreateJsonReference(parameterName), &externalConfig->getExternalData(), this->getRootDirectoryForExternalDataForConstructorArgument()};
    }
    return ParametersWithExternalConfig(&this->getCreateJsonReference(parameterName), this->getRootDirectoryForExternalDataForConstructorArgument());
}

ParametersWithExternalConfig ParametersWithExternalConfig::at(string const &parameterName) {
    ParametersWithExternalConfig *externalConfig;
    if (mapGetIfContains(this->getExternalData().externalKeyToParametersAssociation, parameterName, externalConfig)) {
        return {&this->getJsonReference(parameterName), &externalConfig->getExternalData(), this->getRootDirectoryForExternalDataForConstructorArgument()};
    }
    if (mapGetIfContains(this->getExternalData().externalConfigs, parameterName, externalConfig)) {
        return {&this->getJsonReference(parameterName), &externalConfig->getExternalData(), this->getRootDirectoryForExternalDataForConstructorArgument()};
    }
    return {&this->getJsonReference(parameterName), nullptr, this->getRootDirectoryForExternalDataForConstructorArgument()};
}

void ParametersWithExternalConfig::writeParameters(std::string const &fileName, bool withWriteSubConfigs,
                                                   bool keepOrder, bool keepNewLines) const {
    this->writeParameters(fileName, withWriteSubConfigs, keepOrder ? fileName : "", keepNewLines);
}

void ParametersWithExternalConfig::writeParameters(std::string const &fileName, bool withWriteSubConfigs,
                                                   std::string const &templateContentFile, bool keepNewLines) const {
    nlohmann::json toWriteParameters;
    this->updateParameters(toWriteParameters, withWriteSubConfigs, templateContentFile.empty(), keepNewLines);
    // cout << "Writing " << toWriteParameters.dump(4) << " to file " << fileName << endl;
    writeJsonFile(fileName, toWriteParameters, templateContentFile, keepNewLines);
}

std::string ParametersWithExternalConfig::toString(  // NOLINT(misc-no-recursion)
        string const &indent, bool verbose) const {
    return this->toStringPrivate({}, indent, verbose);
}

void ParametersWithExternalConfig::clear() {
    this->Parameters::clear();
    this->rootDirectoryForExternalData.clear();
    this->externalFilesRelativeToFileLocation = true;
    this->externalDataPtr = nullptr;
    this->isExternalDataReference = false;
    this->externalData.clear();
}

ParametersWithExternalConfig ParametersWithExternalConfig::cloneAsExternal() const {
    return {this->getJson(), this->rootDirectoryForExternalData, this->externalFilesRelativeToFileLocation};
}

ParametersWithExternalConfig::ParametersWithExternalConfig(
    nlohmann::json config, std::string rootDirectoryForExternalData,
    bool externalFilesRelativeToFileLocation) : ParametersWithExternalConfig() {
    this->rootDirectoryForExternalData = std::move(rootDirectoryForExternalData);
    this->externalFilesRelativeToFileLocation = externalFilesRelativeToFileLocation;
    this->initialize(&config, false);
}

ParametersWithExternalConfig::ParametersWithExternalConfig( // NOLINT(misc-no-recursion)
    nlohmann::json *config, std::string rootDirectoryForExternalData,
    bool externalFilesRelativeToFileLocation) : ParametersWithExternalConfig() {
    this->rootDirectoryForExternalData = std::move(rootDirectoryForExternalData);
    this->externalFilesRelativeToFileLocation = externalFilesRelativeToFileLocation;
    this->initialize(config, true);
}

ParametersWithExternalConfig::ParametersWithExternalConfig(
        nlohmann::json config, ExternalParameterData *externalData, std::string rootDirectoryForExternalData) :
        Parameters(std::move(config)), externalDataPtr(externalData), isExternalDataReference(externalData != nullptr),
        externalFilesRelativeToFileLocation(rootDirectoryForExternalData.empty()) {
    if (!this->externalFilesRelativeToFileLocation) {
        this->rootDirectoryForExternalData = std::move(rootDirectoryForExternalData);
    }
}

ParametersWithExternalConfig::ParametersWithExternalConfig(
        nlohmann::json *config, ExternalParameterData *externalData, std::string rootDirectoryForExternalData) :
        Parameters(config), externalDataPtr(externalData), isExternalDataReference(externalData != nullptr),
        externalFilesRelativeToFileLocation(rootDirectoryForExternalData.empty()) {
    if (!this->externalFilesRelativeToFileLocation) {
        this->rootDirectoryForExternalData = std::move(rootDirectoryForExternalData);
    }
}

void ParametersWithExternalConfig::initialize(nlohmann::json *config, bool setReference) {  // NOLINT(misc-no-recursion)
    ExternalParameterData &external = this->getExternalData();
    if (config != nullptr && config->contains(ParametersWithExternalConfig::externalConfigKey)) {
        external.isExternalConfig = true;
        external.externalFileName = config->at("externalConfig");
        external.originalExternalFileName = external.externalFileName;
        if (!isFilePathAbsolute(external.externalFileName)) {
            external.externalFileName = joinToPath({this->rootDirectoryForExternalData, external.externalFileName});
        }
        cout << "Found external config file: " << external.externalFileName << endl;
        auto tmp = ParametersWithExternalConfig(external.externalFileName,
                                                this->getRootDirectoryForExternalDataForConstructorArgument());
        this->parameters = std::move(tmp.parameters);
        ExternalParameterData &tmpExternal = tmp.getExternalData();
        external.externalConfigs = std::move(tmpExternal.externalConfigs);
        external.externalParameters = std::move(tmpExternal.externalParameters);
        external.externalKeyToParametersAssociation = std::move(tmpExternal.externalKeyToParametersAssociation);
        external.externalFileAssociation = std::move(tmpExternal.externalFileAssociation);
        external.externalParameterKeyAssociation = std::move(tmpExternal.externalParameterKeyAssociation);
        if (tmp.isExternalConfig()) {
            external.externalFileName = std::move(tmpExternal.externalFileName);
        }

        if (setReference) {
            *config = std::move(this->parameters);  // important to move the data to keep the pointer-references intact!
            this->parameterReference = config;
            this->isReference = true;
        }
        this->updateExternalParameters(&this->getJsonReference());
    } else {
        if (setReference) {
            this->parameterReference = config;
            this->isReference = (this->parameterReference != nullptr);
        } else {
            assert(config != nullptr);
            this->parameters = std::move(*config);
        }

        auto &jsonConfig = this->getJsonReference();
        if (jsonConfig.is_object()) {
            // create map of external configs
            bool hasExternalConfigs = false;
            for (auto &jsonData: jsonConfig.items()) {
                auto &jsonValue = jsonData.value();
                // use the private constructor
                ParametersWithExternalConfig subConfig(&jsonValue, this->rootDirectoryForExternalData, this->externalFilesRelativeToFileLocation);
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
                        externalFile = joinToPath({this->rootDirectoryForExternalData, externalFile});
                    }
                    auto tmp = ParametersWithExternalConfig(externalFile, this->getRootDirectoryForExternalDataForConstructorArgument());
                    if (tmp.isExternalConfig()) {
                        throw runtime_error("External configs are not allowed on the same layer as external data! " +
                                            ("Write the external config file instead of \"" + originalExternalFile) +
                                            "\" in the external data list!");
                    }

                    // assert that keys in tmp are not in jsonConfig (i.e. not in this level!)
                    map<string, bool> externalKeys;
                    for (auto const &p: tmp.getJson().items()) {
                        mapEmplace(externalKeys, p.key(), true);
                        if (jsonConfig.contains(p.key())) {
                            throw std::runtime_error(
                                    "External config \"" + externalFile + "\" has the same parameter \"" + p.key() +
                                    "\" as this level! Bad configuration!");
                        }
                    }
                    // update the parameters of this config
                    jsonConfig.update(tmp.getJson());
                    // set references of sub-parameters!
                    tmp.updateExternalParameters(&jsonConfig);

                    auto *parameterPtr = &(mapEmplace(external.externalParameters, originalExternalFile,
                                                      std::move(tmp))->second);
                    mapEmplace(external.externalFileAssociation, parameterPtr, originalExternalFile, externalFile);
                    for (auto const &key: externalKeys) {
                        mapEmplace(external.externalKeyToParametersAssociation, key.first, parameterPtr);
                    }
                    mapEmplace(external.externalParameterKeyAssociation, parameterPtr, std::move(externalKeys));
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
        // handle the case when the overwritten parameter contains "externalConfig" data!
        // use the private constructor
        ParametersWithExternalConfig subConfig(
            &this->getJsonReference(parameterName), this->rootDirectoryForExternalData,
            this->externalFilesRelativeToFileLocation);
        if (subConfig.isExternalConfig() || !subConfig.getExternalData().externalConfigs.empty()) {
            *overwrittenConfig = std::move(subConfig);
        }
    }
}

void ParametersWithExternalConfig::toStringMap(  // NOLINT(misc-no-recursion)
        std::stringstream &ss, std::string const &key, nlohmann::json const &value, std::string const &indent,
        std::map<ParametersWithExternalConfig const *, bool> &processedExternalParameters,
        ExternalParameterData const &external, bool verbose) {
    ParametersWithExternalConfig const *p;
    if (AndreiUtils::mapGetIfContains(external.externalConfigs, key, p)) {
        ss << indent + AndreiUtils::tab << key << ": ";
        ss << "{" << endl << p->toStringPrivate({}, indent + AndreiUtils::tab * 2, verbose) << indent + AndreiUtils::tab
           << "}" << endl;
    } else if (AndreiUtils::mapGetIfContains(external.externalKeyToParametersAssociation, key, p)) {
        if (!mapContains(processedExternalParameters, p)) {
            ss << indent + AndreiUtils::tab << "EXTERNAL: "
               << AndreiUtils::mapGet(external.externalFileAssociation, p).second << endl;
            ss << p->toStringPrivate(AndreiUtils::mapGet(external.externalParameterKeyAssociation, p),
                                     indent + AndreiUtils::tab * 2, verbose) << endl;
            mapEmplace(processedExternalParameters, p, true);
        }
    } else {
        ss << indent + AndreiUtils::tab << key << ": ";
        ss << value.dumpWithExistingIndent(4, ' ', int(indent.size()) + 4) << endl;
    }
}

std::string ParametersWithExternalConfig::toStringPrivate(  // NOLINT(misc-no-recursion)
        map<std::string, bool> const &collectOnlyTheseKeys, string const &indent, bool verbose) const {
    stringstream ss;
    ExternalParameterData const &external = this->getExternalData();
    if (verbose) {
        ss << indent << (this->isReference ? "REFERENCE" : "VALUE") << ": ";
        if (this->isExternalConfig()) {
            ss << "External file " << external.externalFileName << ": ";
        }
        ss << this->rootDirectoryForExternalData << ": " << endl;
    }
    json const &jsonData = this->getJson();
    // cout << "Local Json Data: " << jsonData.dump(4) << endl;
    if (jsonData.is_object()) {
        map<ParametersWithExternalConfig const *, bool> processedExternalParameters;
        if (collectOnlyTheseKeys.empty()) {
            for (auto const &jsonDatum: jsonData.items()) {
                this->toStringMap(ss, jsonDatum.key(), jsonDatum.value(), indent, processedExternalParameters, external,
                                  verbose);
            }
        } else {
            for (auto const &key: collectOnlyTheseKeys) {
                this->toStringMap(ss, key.first, jsonData.at(key.first), indent, processedExternalParameters, external,
                                  verbose);
            }
        }
    } else {
        if (!external.externalConfigs.empty()) {
            // this should never happen!
            ss << jsonData.dump(4) << endl;
            cout << ss.str() << endl;
            assert(external.externalConfigs.empty());
        }
        ss << indent + AndreiUtils::tab << jsonData.dumpWithExistingIndent(4, ' ', (int) indent.size() / 4) << endl;
    }
    return ss.str();
}

void ParametersWithExternalConfig::updateParameters(  // NOLINT(misc-no-recursion)
        nlohmann::json &toWriteParameters, bool recurseSubConfigs, bool keepOrder, bool keepNewLines,
        std::map<std::string, bool> const &collectOnlyTheseParameters) const {
    nlohmann::json theseParameters;
    this->collectAndUpdateParametersToWriteForThisFile(theseParameters, recurseSubConfigs, keepOrder,
                                                       keepNewLines, collectOnlyTheseParameters);
    ExternalParameterData const &external = this->getExternalData();
    if (!external.externalParameters.empty()) {
        for (auto const &externalParameters: external.externalParameters) {
            auto processedExternalFile = mapGet(external.externalFileAssociation, &externalParameters.second).second;
            nlohmann::json toWriteExternalParameters;
            externalParameters.second.updateParameters(
                    toWriteExternalParameters, recurseSubConfigs, keepOrder, keepNewLines,
                    mapGet(external.externalParameterKeyAssociation, &externalParameters.second));
            writeJsonFile(processedExternalFile, toWriteExternalParameters, keepOrder ? processedExternalFile : "",
                          keepNewLines);
        }
        theseParameters[ParametersWithExternalConfig::externalDataKey] = getMapKeys(external.externalParameters);
    }
    if (this->isExternalConfig()) {
        toWriteParameters[ParametersWithExternalConfig::externalConfigKey] = external.originalExternalFileName;
        // cout << "Writing parameters to file " << external.externalFileName << ": "<< theseParameters.dump(4) << endl;
        writeJsonFile(external.externalFileName, theseParameters, keepOrder ? external.externalFileName : "",
                      keepNewLines);
    } else {
        toWriteParameters = std::move(theseParameters);
    }
}

void ParametersWithExternalConfig::collectAndUpdateParametersToWriteForThisFileObjectData(  // NOLINT(misc-no-recursion)
        std::string const &datumKey, nlohmann::json const &datumValue, ExternalParameterData const &external,
        json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines) {
    ParametersWithExternalConfig const *subConfig;
    if (AndreiUtils::mapGetIfContains(external.externalConfigs, datumKey, subConfig)) {
        json subConfigJson;
        subConfig->updateParameters(subConfigJson, recurseSubConfigs, keepOrder, keepNewLines);
        parametersToWrite[datumKey] = subConfigJson;
    } else if (!AndreiUtils::mapContains(external.externalKeyToParametersAssociation, datumKey)) {
        parametersToWrite[datumKey] = datumValue;
    }
}

void ParametersWithExternalConfig::collectAndUpdateParametersToWriteForThisFile(  // NOLINT(misc-no-recursion)
        json &parametersToWrite, bool recurseSubConfigs, bool keepOrder, bool keepNewLines,
        std::map<std::string, bool> const &collectOnlyTheseKeys) const {
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        ExternalParameterData const &external = this->getExternalData();
        if (collectOnlyTheseKeys.empty()) {
            for (auto const &datum: jsonData.get<std::map<std::string, nlohmann::json>>()) {
                this->collectAndUpdateParametersToWriteForThisFileObjectData(
                        datum.first, datum.second, external, parametersToWrite, recurseSubConfigs, keepOrder,
                        keepNewLines);
            }
        } else {
            for (auto const &key: collectOnlyTheseKeys) {
                this->collectAndUpdateParametersToWriteForThisFileObjectData(
                        key.first, jsonData.at(key.first), external, parametersToWrite, recurseSubConfigs, keepOrder,
                        keepNewLines);
            }
        }
    } else {
        parametersToWrite = jsonData;
    }
}

void ParametersWithExternalConfig::updateExternalParameters(  // NOLINT(misc-no-recursion)
        nlohmann::json *parameterParent) {
    for (auto &externalParameter: this->getExternalData().externalParameters) {
        externalParameter.second.updateExternalParameters(parameterParent);
    }
    for (auto &externalConfig: this->getExternalData().externalConfigs) {
        externalConfig.second.updateExternalParameters(&parameterParent->at(externalConfig.first));
    }
    assert(!this->isExternalConfig() || this->isReference);
    this->isReference = true;
    this->parameterReference = parameterParent;
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

std::string ParametersWithExternalConfig::getRootDirectoryForExternalDataForConstructorArgument() const {
    return this->externalFilesRelativeToFileLocation ? "" : this->rootDirectoryForExternalData;
}

ParametersWithExternalConfig::ExternalParameterData::ExternalParameterData() : isExternalConfig(false) {}

void ParametersWithExternalConfig::ExternalParameterData::clear() {
    this->externalConfigs.clear();
    this->externalParameters.clear();
    this->externalFileAssociation.clear();
    this->externalParameterKeyAssociation.clear();
    this->externalKeyToParametersAssociation.clear();
    this->isExternalConfig = false;
    this->externalFileName.clear();
    this->originalExternalFileName.clear();
}
