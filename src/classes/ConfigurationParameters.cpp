//
// Created by Andrei on 10.11.22.
//

#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/utilsJson.h>
#include <AndreiUtils/utilsString.h>
#include <iostream>
#include <stdexcept>

using namespace AndreiUtils;
using namespace nlohmann;
using namespace std;

ConfigurationParameters::ConfigurationParameters(string const &fileName, std::string parametersFor) :
        ConfigurationParameters(readJsonFile(fileName), std::move(parametersFor)) {}

ConfigurationParameters::ConfigurationParameters(json config, std::string parametersFor) :
        initialized(true), parametersFor(std::move(parametersFor)) {
    if (!this->checkCorrectJsonParameters(config)) {
        string s = config.dump(4);
        throw std::runtime_error(
                "Configuration:\n" + s + "\n does not contain parameters for \"" + this->parametersFor + "\"");
    }
    this->config = std::move(config);
}

ConfigurationParameters::ConfigurationParameters() noexcept: initialized(false) {}

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters const &other) = default;

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters &&other) noexcept:
        config(std::move(other.config)), initialized(other.initialized), parametersFor(std::move(other.parametersFor)) {
    other.initialized = false;
}

ConfigurationParameters &ConfigurationParameters::operator=(ConfigurationParameters const &other) {
    if (this != &other) {
        this->config = other.config;
        this->initialized = other.initialized;
        this->parametersFor = other.parametersFor;
    }
    return *this;
}

ConfigurationParameters &ConfigurationParameters::operator=(ConfigurationParameters &&other) noexcept {
    if (this != &other) {
        this->config = std::move(other.config);
        this->initialized = other.initialized;
        other.initialized = false;
        this->parametersFor = std::move(other.parametersFor);
    }
    return *this;
}

bool ConfigurationParameters::operator==(ConfigurationParameters const &other) const {
    if (this->initialized != other.initialized) {
        return false;
    }
    if (this->parametersFor != other.parametersFor) {
        return false;
    }
    return this->config == other.config;
}

bool ConfigurationParameters::operator!=(ConfigurationParameters const &other) const {
    return !(*this == other);
}

bool ConfigurationParameters::isInitialized() const {
    return this->initialized;
}

bool ConfigurationParameters::has(string const &parameterName) const {
    return this->config.contains(parameterName);
}

ConfigurationParameters ConfigurationParameters::getSubConfig(string const &subParametersFor) const {
    if (!AndreiUtils::contains(subParametersFor, "/")) {
        return ConfigurationParameters(this->config.at(subParametersFor), subParametersFor);
    }
    ConfigurationParameters subConfig = *this;
    for (auto const &subSubParametersFor: AndreiUtils::splitString(subParametersFor, "/")) {
        subConfig = ConfigurationParameters(subConfig.config.at(subSubParametersFor), subSubParametersFor);
    }
    return subConfig;
}

bool ConfigurationParameters::checkCorrectJsonParameters(json &_config) const {
    try {
        if (!this->parametersFor.empty()) {
            if (_config.contains("parametersFor") && _config.at("parametersFor").get<string>() == this->parametersFor) {
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

json const &ConfigurationParameters::getJson(string const &parameterName) const {
    if (!this->initialized) {
        throw std::runtime_error("ConfigurationParameters not initialized!");
    }
    return this->config.at(parameterName);
}

json &ConfigurationParameters::getJson() {
    return this->config;
}

json const &ConfigurationParameters::getJson() const {
    return this->config;
}
