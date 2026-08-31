/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        throw std::runtime_error("Configuration:\n" + s + "\n does not contain parameters for \"" +
                                 this->parametersFor + "\"");
    }
    this->parameters = std::move(config);
}

ConfigurationParameters::ConfigurationParameters() noexcept : Parameters() {}

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters const &other) = default;

ConfigurationParameters::ConfigurationParameters(ConfigurationParameters &&other) noexcept :
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
        this->parametersFor = std::move(other.parametersFor); // NOLINT(bugprone-use-after-move)
    }
    return *this;
}

bool ConfigurationParameters::operator==(ConfigurationParameters const &other) const {
    if (this->parametersFor != other.parametersFor) {
        return false;
    }
    return this->Parameters::operator==(other);
}

bool ConfigurationParameters::operator!=(ConfigurationParameters const &other) const { return !(*this == other); }

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

void ConfigurationParameters::clear() {
    this->Parameters::clear();
    this->parametersFor.clear();
}

bool ConfigurationParameters::checkCorrectJsonParameters(json &_config) const { // NOLINT(misc-no-recursion)
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
