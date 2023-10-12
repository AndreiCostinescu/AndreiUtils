//
// Created by Andrei on 05.10.23.
//

#include <AndreiUtils/classes/UserInteraction.hpp>
#include <AndreiUtils/exceptions/DebugRequestedException.h>
#include <AndreiUtils/exceptions/QuitRequestedException.h>
#include <AndreiUtils/utils.h>
#include <AndreiUtils/utilsString.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

bool UserInteraction::getBooleanSupervision(std::string const &prompt, std::function<UserResponse()> const &f) {
    return UserInteraction::getBooleanSupervisionWithScenario(prompt, f);
}

int UserInteraction::getIndexSupervision(string const &prompt, int minIndex, int maxIndex, function<int()> const &f) {
    return UserInteraction::getIndexSupervisionWithScenario(prompt, minIndex, maxIndex, f);
}

std::string UserInteraction::getStringSupervision(string const &prompt, bool allowEmpty, function<string()> const &f) {
    return UserInteraction::getStringSupervisionWithScenario(prompt, allowEmpty, f);
}

UserInteraction::UserInteraction(std::string const &scenario) {
    if (!scenario.empty()) {
        this->scenario = make_shared<ifstream>(scenario);
    }
}

UserInteraction::UserInteraction(UserInteraction const &other) : ss(other.ss.str()) {}

UserInteraction::UserInteraction(UserInteraction &&other) noexcept: ss(std::move(other.ss)) {}

UserInteraction::~UserInteraction() {
    this->clear();
    if (this->useScenario()) {
        this->scenario->close();
    }
}

UserInteraction &UserInteraction::operator=(UserInteraction const &other) {
    if (this != &other) {
        this->ss.str(other.ss.str());
    }
    return *this;
}

UserInteraction &UserInteraction::operator=(UserInteraction &&other) noexcept {
    if (this != &other) {
        this->ss = std::move(other.ss);
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

bool UserInteraction::getBooleanResponse(function<UserResponse()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
        } else {
            this->scenario->close();
        }
        return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), f, scenarioResponse);
    }
    return UserInteraction::getBooleanSupervisionWithScenario(this->ss.str(), f);
}

int UserInteraction::getIndexResponse(int minIndex, int maxIndex, function<int()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
        } else {
            this->scenario->close();
        }
        return UserInteraction::getIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex, f,
                                                                scenarioResponse);
    }
    return UserInteraction::getIndexSupervisionWithScenario(this->ss.str(), minIndex, maxIndex, f);
}

std::string UserInteraction::getStringResponse(bool allowEmpty, function<string()> const &f) const {
    if (this->useScenario()) {
        string scenarioResponse;
        if (!getline(*this->scenario, scenarioResponse)) {
            scenarioResponse.clear();
        } else {
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
        string const &prompt, function<UserResponse()> const &f, string const &scenarioResponse) {
    UserResponse res;
    while (true) {
        cout << prompt << ((endsWith(prompt, "\n") && !endsWith(prompt, " ")) ? "" : " ") << "Type 'yes' or 'no': ";
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
            if (response == "yes" || response == "y" || response == "1") {
                res = UserResponse_OK;
            } else if (response == "no" || response == "n" || response == "0") {
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
