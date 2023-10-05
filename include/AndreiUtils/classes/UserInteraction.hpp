//
// Created by andrei on 05.10.23.
//

#pragma once

#include <functional>
#include <sstream>
#include <string>

namespace AndreiUtils {
    class UserInteraction {
    public:
        enum UserResponse {
            UserResponse_OK,
            UserResponse_NOT_OK,
            UserResponse_NO_RESPONSE,
        };

        static bool getBooleanSupervision(std::string const &prompt, std::function<UserResponse()> const &f = {});

        static int getIndexSupervision(std::string const &prompt, int minIndex, int maxIndex,
                                       std::function<int()> const &f = {});

        static std::string getStringSupervision(std::string const &prompt, bool allowEmpty,
                                                std::function<std::string()> const &f = {});

        UserInteraction();

        UserInteraction(UserInteraction const &other);

        UserInteraction(UserInteraction &&other) noexcept;

        ~UserInteraction();

        UserInteraction &operator=(UserInteraction const &other);

        UserInteraction &operator=(UserInteraction &&other) noexcept;

        UserInteraction &clear();

        UserInteraction &operator()();

        template<typename T>
        UserInteraction &operator<<(T const &data) {
            this->ss << data;
            return *this;
        }

        [[nodiscard]] bool getBooleanResponse(std::function<UserResponse()> const &f = {}) const;

        [[nodiscard]] int getIndexResponse(int minIndex, int maxIndex, std::function<int()> const &f = {}) const;

        [[nodiscard]] std::string getStringResponse(bool allowEmpty, std::function<std::string()> const &f = {}) const;

    protected:
        std::stringstream ss;
    };
}