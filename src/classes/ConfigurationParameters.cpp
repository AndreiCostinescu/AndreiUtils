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
