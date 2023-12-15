//
// Created by Andrei on 10.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/utilsFiles.h>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsMap.hpp>
#include <AndreiUtils/utilsString.h>
#include <iostream>
#include <stdexcept>

using namespace AndreiUtils;
using namespace std;

using json = nlohmann::json;

Parameters::Parameters() : parameters(), parameterReference(nullptr), isReference(false) {}

Parameters::Parameters(nlohmann::json config) : parameters(std::move(config)), parameterReference(nullptr),
                                                isReference(false) {}

Parameters::Parameters(nlohmann::json *config) : parameterReference(config), parameters(),
                                                 isReference(config != nullptr) {}

Parameters::Parameters(Parameters const &other) = default;

Parameters::Parameters(Parameters &&other) noexcept = default;

Parameters::~Parameters() noexcept = default;

Parameters &Parameters::operator=(Parameters const &other) {
    if (this != &other) {
        this->parameters = other.parameters;
        this->parameterReference = other.parameterReference;
        this->isReference = other.isReference;
    }
    return *this;
}

Parameters &Parameters::operator=(Parameters &&other) noexcept {
    if (this != &other) {
        this->parameters = std::move(other.parameters);
        this->parameterReference = other.parameterReference;
        this->isReference = other.isReference;
    }
    return *this;
}

bool Parameters::operator==(Parameters const &other) const {
    if (this->isReference != other.isReference) {
        return false;
    }
    if (!this->isReference) {
        return this->parameters == other.parameters;
    }
    if (this->parameterReference == nullptr || other.parameterReference == nullptr) {
        // if both are nullptr then the parameters are the same
        return this->parameterReference == other.parameterReference;
    }
    // now both references are != nullptr
    return (*this->parameterReference) == (*other.parameterReference);
}

bool Parameters::operator!=(Parameters const &other) const {
    return !(*this == other);
}

bool Parameters::empty() const {
    return this->isReference ? (this->parameterReference == nullptr || this->parameterReference->empty())
                             : this->parameters.empty();
}

bool Parameters::has(string const &parameterName) const {
    return this->isReference ? (this->parameterReference != nullptr &&
                                this->parameterReference->contains(parameterName))
                             : this->parameters.contains(parameterName);
}

void Parameters::set(nlohmann::json const &data) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not set json value in a nullptr reference!");
        }
        *this->parameterReference = data;
    } else {
        this->parameters = data;
    }
}

void Parameters::set(nlohmann::json &&data) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not set json value in a nullptr reference!");
        }
        *this->parameterReference = std::move(data);
    } else {
        this->parameters = std::move(data);
    }
}

bool Parameters::deleteKey(std::string const &parameterName) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not delete a json value from a nullptr reference!");
        }
        return this->parameterReference->erase(parameterName);
    }
    return this->parameters.erase(parameterName);
}

Parameters Parameters::operator[](std::string const &parameterName) {
    return Parameters(&this->getCreateJsonReference(parameterName));
}

Parameters Parameters::at(string const &parameterName) {
    return Parameters(&this->getJsonReference(parameterName));
}

nlohmann::json const &Parameters::getCreateJson(string const &parameterName) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get-create json value from a nullptr reference!");
        }
        return (*this->parameterReference)[parameterName];
    }
    return this->parameters[parameterName];
}

nlohmann::json const &Parameters::getJson(string const &parameterName) const {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get json value from a nullptr reference!");
        }
        return this->parameterReference->at(parameterName);
    }
    return this->parameters.at(parameterName);
}

nlohmann::json const &Parameters::getJson() const {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get json value from a nullptr reference!");
        }
        return *this->parameterReference;
    }
    return this->parameters;
}

nlohmann::json &Parameters::getCreateJsonReference(std::string const &parameterName) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get-create json value from a nullptr reference!");
        }
        return (*this->parameterReference)[parameterName];
    }
    return this->parameters[parameterName];
}

nlohmann::json &Parameters::getJsonReference(string const &parameterName) {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get json value from a nullptr reference!");
        }
        return this->parameterReference->at(parameterName);
    }
    return this->parameters.at(parameterName);
}

nlohmann::json &Parameters::getJsonReference() {
    if (this->isReference) {
        if (this->parameterReference == nullptr) {
            throw std::runtime_error("Can not get json value from a nullptr reference!");
        }
        return *this->parameterReference;
    }
    return this->parameters;
}

std::string const ConfigurationParameters::parameterForKey("parametersFor");

ConfigurationParameters::ConfigurationParameters(string const &fileName, std::string parametersFor) :
        ConfigurationParameters(readJsonFile(fileName), std::move(parametersFor)) {}

ConfigurationParameters::ConfigurationParameters(json config, std::string parametersFor) :
        Parameters(), parametersFor(std::move(parametersFor)) {
    if (!this->checkCorrectJsonParameters(config)) {
        string s = config.dump(4);
        throw std::runtime_error(
                "Configuration:\n" + s + "\n does not contain parameters for \"" + this->parametersFor + "\"");
    }
    this->parameters = std::move(config);
}

ConfigurationParameters::ConfigurationParameters() noexcept: Parameters() {}

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters const &other) = default;

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters &&other) noexcept:
        Parameters(std::move(other)), parametersFor(std::move(other.parametersFor)) {}

ConfigurationParameters &ConfigurationParameters::operator=(ConfigurationParameters const &other) {
    if (this != &other) {
        this->Parameters::operator=(other);
        this->parametersFor = other.parametersFor;
    }
    return *this;
}

ConfigurationParameters &ConfigurationParameters::operator=(ConfigurationParameters &&other) noexcept {
    if (this != &other) {
        this->Parameters::operator=(std::move(other));
        this->parametersFor = std::move(other.parametersFor);  // NOLINT(bugprone-use-after-move)
    }
    return *this;
}

bool ConfigurationParameters::operator==(ConfigurationParameters const &other) const {
    if (this->parametersFor != other.parametersFor) {
        return false;
    }
    return this->Parameters::operator==(other);
}

bool ConfigurationParameters::operator!=(ConfigurationParameters const &other) const {
    return !(*this == other);
}

ConfigurationParameters ConfigurationParameters::getSubConfig(string const &subParametersFor) const {
    if (!AndreiUtils::contains(subParametersFor, "/")) {
        return ConfigurationParameters(this->getJson(subParametersFor), subParametersFor);
    }
    ConfigurationParameters subConfig = *this;
    for (auto const &subSubParametersFor: AndreiUtils::splitString(subParametersFor, "/")) {
        subConfig = ConfigurationParameters(subConfig.getJson(subSubParametersFor), subSubParametersFor);
    }
    return subConfig;
}

bool ConfigurationParameters::checkCorrectJsonParameters(json &_config) const {  // NOLINT(misc-no-recursion)
    try {
        if (!this->parametersFor.empty()) {
            if (_config.contains(ConfigurationParameters::parameterForKey) &&
                _config.at(ConfigurationParameters::parameterForKey).get<string>() == this->parametersFor) {
                return true;
            }
            if (_config.contains(this->parametersFor + "ConfigFile")) {
                json externalConfig(readJsonFile(_config.at(this->parametersFor + "ConfigFile").get<string>()));
                if (this->checkCorrectJsonParameters(externalConfig)) {
                    _config = externalConfig;
                    return true;
                }
            }
        }
    } catch (exception &e) {
        cout << "Caught exception checking for correct configuration parameters: " << e.what() << endl;
    }
    return false;
}

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

bool ParametersWithExternalConfig::deleteKey(std::string const &parameterName) {
    if (this->Parameters::deleteKey(parameterName)) {
        // bool res = AndreiUtils::mapDelete(this->externalConfigs, parameterName); assert(res);
        AndreiUtils::mapDelete(this->externalConfigs, parameterName);
        return true;
    } else {
        return false;
    }
}

void ParametersWithExternalConfig::writeParameters(std::string const &fileName, bool withWriteSubConfigs) const {
    nlohmann::json toWriteParameters;
    this->updateParameters(toWriteParameters, withWriteSubConfigs);
    // cout << "Writing " << toWriteParameters.dump(4) << " to file " << fileName << endl;
    writeJsonFile(fileName, toWriteParameters);
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
        nlohmann::json &toWriteParameters, bool withWriteSubConfigs) const {
    nlohmann::json theseParameters;
    this->collectAndUpdateParametersToWriteForThisFile(theseParameters, withWriteSubConfigs);
    if (this->isExternalConfig) {
        toWriteParameters[ParametersWithExternalConfig::externalConfigKey] = this->originalExternalFileName;
        // cout << "Writing " << theseParameters.dump(4) << " to file " << this->externalFileName << endl;
        writeJsonFile(this->externalFileName, theseParameters);
    } else {
        toWriteParameters = std::move(theseParameters);
    }
}

void ParametersWithExternalConfig::collectAndUpdateParametersToWriteForThisFile(  // NOLINT(misc-no-recursion)
        json &parametersToWrite, bool recurse) const {
    json const &jsonData = this->getJson();
    if (jsonData.is_object()) {
        for (auto const &datum: jsonData.items()) {
            ParametersWithExternalConfig const *subConfig;
            if (AndreiUtils::mapGetIfContains(this->externalConfigs, datum.key(), subConfig)) {
                json subConfigJson;
                subConfig->updateParameters(subConfigJson, recurse);
                parametersToWrite[datum.key()] = subConfigJson;
            } else {
                parametersToWrite[datum.key()] = datum.value();
            }
        }
    } else {
        parametersToWrite = jsonData;
    }
}
