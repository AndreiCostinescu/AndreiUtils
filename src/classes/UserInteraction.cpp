//
// Created by Andrei on 05.10.23.
//

#include <AndreiUtils/classes/UserInteraction.hpp>
#include <AndreiUtils/exceptions/DebugRequestedException.h>
#include <AndreiUtils/exceptions/QuitRequestedException.h>
#include <AndreiUtils/utils.h>
#include <AndreiUtils/utilsString.h>
#include <cassert>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

bool UserInteraction::getBooleanSupervision(std::string const &prompt, std::function<UserResponse()> const &f) {
    return UserInteraction::getBooleanSupervisionWithScenario(prompt, false, f);
}

bool UserInteraction::getBooleanSupervision(std::string const &prompt, bool expectBooleanValues,
                                            std::function<UserResponse()> const &f) {
    return UserInteraction::getBooleanSupervisionWithScenario(prompt, expectBooleanValues, f);
}

int UserInteraction::getIndexSupervision(string const &prompt, int minIndex, int maxIndex, function<int()> const &f) {
    return UserInteraction::getIndexSupervisionWithScenario(prompt, minIndex, maxIndex, f);
}

vector<int> UserInteraction::getMultipleIndexSupervision(
        string const &prompt, int minIndex, int maxIndex, bool allowEmptyResponse, function<vector<int>()> const &f) {
    return UserInteraction::getMultipleIndexSupervisionWithScenario(prompt, minIndex, maxIndex, allowEmptyResponse, f);
}

std::string UserInteraction::getStringSupervision(string const &prompt, bool allowEmpty, function<string()> const &f) {
    return UserInteraction::getStringSupervisionWithScenario(prompt, allowEmpty, f);
}

UserInteraction::UserInteraction(std::string const &scenario) {
    this->setScenario(scenario);
}

UserInteraction::UserInteraction(UserInteraction const &other) : ss(other.ss.str()), scenario(other.scenario) {}

UserInteraction::UserInteraction(UserInteraction &&other) noexcept:
        ss(std::move(other.ss)), scenario(std::move(other.scenario)) {}

UserInteraction::~UserInteraction() {
    this->clear();
    if (this->useScenario()) {
        this->scenario->close();
    }
}

UserInteraction &UserInteraction::operator=(UserInteraction const &other) {
    if (this != &other) {
        this->ss.str(other.ss.str());
        this->scenario = other.scenario;
    }
    return *this;
}

UserInteraction &UserInteraction::operator=(UserInteraction &&other) noexcept {
    if (this != &other) {
        this->ss = std::move(other.ss);
        this->scenario = std::move(other.scenario);
    }
    return *this;
}

UserInteraction &UserInteraction::clear() {
    this->ss.str("");
    this->ss.clear();
    return *this;
}

UserInteraction &UserInteraction::operator()() {
    return this->clear();
}

void UserInteraction::setScenario(string const &scenarioFile) {
    if (!scenarioFile.empty()) {
        this->scenario = make_shared<ifstream>(scenarioFile);
        assert(this->scenario->is_open());
    }
}

bool UserInteraction::getBooleanResponse(function<UserResponse()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
            this->scenario->close();
        }
        return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), false, f, scenarioResponse);
    }
    return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), false, f);
}

bool UserInteraction::getBooleanResponse(bool expectBooleanValues, function<UserResponse()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
            this->scenario->close();
        }
        return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), expectBooleanValues, f,
                                                                  scenarioResponse);
    }
    return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), expectBooleanValues, f);
}

int UserInteraction::getIndexResponse(int minIndex, int maxIndex, function<int()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
            this->scenario->close();
        }
        return UserInteraction::getIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex, f,
                                                                scenarioResponse);
    }
    return UserInteraction::getIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex, f);
}

vector<int> UserInteraction::getMultipleIndexResponse(
        int minIndex, int maxIndex, bool allowEmptyResponse, function<vector<int>()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
            this->scenario->close();
        }
        return UserInteraction::getMultipleIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex,
                                                                        allowEmptyResponse, f, scenarioResponse);
    }
    return UserInteraction::getMultipleIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex,
                                                                    allowEmptyResponse, f);
}

std::string UserInteraction::getStringResponse(bool allowEmpty, function<string()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
            this->scenario->close();
        }
        return UserInteraction::getStringSupervisionWithScenario(this->ss.str(), allowEmpty, f, scenarioResponse);
    }
    return UserInteraction::getStringSupervisionWithScenario(this->ss.str(), allowEmpty, f);
}

bool UserInteraction::useScenario() const {
    return this->scenario != nullptr && this->scenario->is_open();
}

bool UserInteraction::getBooleanSupervisionWithScenario(
        string const &prompt, bool expectBooleanValues, function<UserResponse()> const &f,
        string const &scenarioResponse) {
    UserResponse res;
    while (true) {
        cout << prompt << ((endsWith(prompt, "\n") && !endsWith(prompt, " ")) ? "" : " ")
             << "Type " << (expectBooleanValues ? "'true' or 'false'" : "'yes' or 'no'") << ": ";
        if (f) {
            res = f();
        } else {
            string response;
            if (scenarioResponse.empty()) {
                getline(cin, response);
            } else {
                cout << scenarioResponse << endl;
                response = scenarioResponse;
            }
            if (AndreiUtils::isStringBooleanTrueValue(response)) {
                res = UserResponse_OK;
            } else if (AndreiUtils::isStringBooleanFalseValue(response)) {
                res = UserResponse_NOT_OK;
            } else if (response == "q" || response == "quit" || response == "exit") {
                throw QuitRequestedException();
            } else {
                res = UserResponse_NO_RESPONSE;
            }
        }
        if (res != UserResponse_NO_RESPONSE) {
            break;
        }
    }
    return res == UserResponse_OK;
}

int UserInteraction::getIndexSupervisionWithScenario(
        string const &prompt, int minIndex, int maxIndex, function<int()> const &f, string const &scenarioResponse) {
    if (!(minIndex < 0 && maxIndex < 0) && maxIndex < minIndex) {
        throw std::runtime_error(
                "In function UserInteraction::getIndexSupervision the maxIndex (" + to_string(maxIndex) +
                ") is lower than the minIndex (" + to_string(minIndex) + ")!");
    }
    int res;
    while (true) {
        cout << prompt;
        if (f) {
            res = f();
        } else {
            string response;
            if (scenarioResponse.empty()) {
                getline(cin, response);
            } else {
                response = scenarioResponse;
                cout << scenarioResponse << endl;
            }
            if (!stringIsInteger(response, res)) {
                if (response == "q" || response == "quit" || response == "exit") {
                    throw QuitRequestedException();
                } else if (response == "d" || response == "debug") {
                    throw DebugRequestedException();
                }
                continue;
            }
        }
        if ((minIndex < 0 && maxIndex < 0) || (minIndex <= res && res <= maxIndex)) {
            break;
        } else {
            cout << "Input error: " << res << " is not in index interval [" << minIndex << ", " << maxIndex << "]"
                 << endl;
        }
    }
    return res;
}

vector<int> UserInteraction::getMultipleIndexSupervisionWithScenario(
        string const &prompt, int minIndex, int maxIndex, bool allowEmptyResponse, function<vector<int>()> const &f,
        string const &scenarioResponse) {
    if (!(minIndex < 0 && maxIndex < 0) && maxIndex < minIndex) {
        throw std::runtime_error(
                "In function UserInteraction::getMultipleIndexSupervision the maxIndex (" + to_string(maxIndex) +
                ") is lower than the minIndex (" + to_string(minIndex) + ")!");
    }
    vector<int> res;
    while (true) {
        res.clear();  // reset content from previous loop iterations
        cout << prompt << "\nIf multiple answers, separate multiple indices by a comma ',' character: ";
        if (f) {
            res = f();
        } else {
            string response;
            if (scenarioResponse.empty()) {
                getline(cin, response);
            } else {
                response = scenarioResponse;
                cout << scenarioResponse << endl;
            }
            if (response == "q" || response == "quit" || response == "exit") {
                throw QuitRequestedException();
            } else if (response == "d" || response == "debug") {
                throw DebugRequestedException();
            }
            auto splitRes = splitString(response, ",");
            bool conversionError = false;
            for (auto const &splitResValue: splitRes) {
                int value;
                if (!stringIsInteger(trim(splitResValue), value)) {
                    cout << "\"" << trim(splitResValue) << "\" is not an index (i.e. integer) value!" << endl;
                    conversionError = true;
                    break;
                }
                res.emplace_back(std::move(value)); // NOLINT(performance-move-const-arg)
            }
            if (conversionError) {
                continue;
            }
        }
        if (!allowEmptyResponse && res.empty()) {
            cout << "Can not accept empty response!" << endl;
            continue;
        }
        bool checkOk = true;
        for (auto const &resValue: res) {
            if ((minIndex < 0 && maxIndex < 0) || (minIndex <= resValue && resValue <= maxIndex)) {
                checkOk = false;
                cout << "Input error: " << resValue << " is not in index interval [" << minIndex << ", " << maxIndex
                     << "]" << endl;
                break;
            }
        }
        if (checkOk) {
            break;
        }
    }
    return res;
}

std::string UserInteraction::getStringSupervisionWithScenario(
        string const &prompt, bool allowEmpty, function<string()> const &f, string const &scenarioResponse) {
    string res;
    while (true) {
        cout << prompt;
        if (f) {
            res = f();
        } else {
            if (scenarioResponse.empty()) {
                getline(cin, res);
            } else {
                res = scenarioResponse;
                cout << scenarioResponse << endl;
            }
            if (res == "q" || res == "quit" || res == "exit") {
                throw QuitRequestedException();
            }
        }
        if (allowEmpty || !res.empty()) {
            break;
        }
    }
    return res;
}
