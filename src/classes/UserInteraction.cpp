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
    UserResponse res;
    while (true) {
        cout << prompt << ((endsWith(prompt, "\n") && !endsWith(prompt, " ")) ? "" : " ") << "Type 'yes' or 'no': ";
        if (f) {
            res = f();
        } else {
            string response;
            getline(cin, response);
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

int UserInteraction::getIndexSupervision(string const &prompt, int minIndex, int maxIndex, function<int()> const &f) {
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
            getline(cin, response);
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

std::string UserInteraction::getStringSupervision(string const &prompt, bool allowEmpty, function<string()> const &f) {
    string res;
    while (true) {
        cout << prompt;
        if (f) {
            res = f();
        } else {
            getline(cin, res);
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

UserInteraction::UserInteraction() = default;

UserInteraction::UserInteraction(UserInteraction const &other) : ss(other.ss.str()) {}

UserInteraction::UserInteraction(UserInteraction &&other) noexcept : ss(std::move(other.ss)) {}

UserInteraction::~UserInteraction() {
    this->clear();
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
    return UserInteraction::getBooleanSupervision(this->ss.str(), f);
}

int UserInteraction::getIndexResponse(int minIndex, int maxIndex, function<int()> const &f) const {
    return UserInteraction::getIndexSupervision(this->ss.str(), minIndex, maxIndex, f);
}

std::string UserInteraction::getStringResponse(bool allowEmpty, function<string()> const &f) const {
    return UserInteraction::getStringSupervision(this->ss.str(), allowEmpty, f);
}
