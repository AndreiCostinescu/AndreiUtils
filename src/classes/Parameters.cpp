//
// Created by Andrei on 18.12.23.
//

#include <AndreiUtils/classes/Parameters.hpp>
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
