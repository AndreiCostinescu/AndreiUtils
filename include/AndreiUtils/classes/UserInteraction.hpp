//
// Created by andrei on 05.10.23.
//

#pragma once

#include <functional>
#include <fstream>
#include <memory>
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

        static bool getBooleanSupervision(std::string const &prompt, bool expectBooleanValues,
                                          std::function<UserResponse()> const &f = {});

        static int getIndexSupervision(
                std::string const &prompt, int minIndex, int maxIndex, std::function<int()> const &f = {});

        static std::vector<int> getMultipleIndexSupervision(
                std::string const &prompt, int minIndex, int maxIndex, bool allowEmptyResponse,
                std::function<std::vector<int>()> const &f = {});

        static std::string getStringSupervision(
                std::string const &prompt, bool allowEmpty, std::function<std::string()> const &f = {});

        explicit UserInteraction(bool clearAfterEachResponse);

        explicit UserInteraction(std::string const &interactionScenario = "");

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

        void setScenario(std::string const &scenarioFile);

        [[nodiscard]] bool getBooleanResponse(std::function<UserResponse()> const &f = {});

        [[nodiscard]] bool getBooleanResponse(
                bool expectBooleanValues, std::function<UserResponse()> const &f = {});

        [[nodiscard]] int getIndexResponse(int minIndex, int maxIndex, std::function<int()> const &f = {});

        [[nodiscard]] std::vector<int> getMultipleIndexResponse(int minIndex, int maxIndex, bool allowEmptyResponse,
                                                                std::function<std::vector<int>()> const &f = {});

        [[nodiscard]] std::string getStringResponse(bool allowEmpty, std::function<std::string()> const &f = {});

        void tell();

    protected:
        static bool getBooleanSupervisionWithScenario(
                std::string const &prompt, bool expectBooleanValues, std::function<UserResponse()> const &f = {},
                std::string const &scenarioResponse = "");

        static int getIndexSupervisionWithScenario(
                std::string const &prompt, int minIndex, int maxIndex, std::function<int()> const &f = {},
                std::string const &scenarioResponse = "");

        static std::vector<int> getMultipleIndexSupervisionWithScenario(
                std::string const &prompt, int minIndex, int maxIndex, bool allowEmptyResponse,
                std::function<std::vector<int>()> const &f = {}, std::string const &scenarioResponse = "");

        static std::string getStringSupervisionWithScenario(
                std::string const &prompt, bool allowEmpty, std::function<std::string()> const &f = {},
                std::string const &scenarioResponse = "");

        [[nodiscard]] bool useScenario() const;

        bool clearAfterEachResponse;
        std::stringstream ss;
        std::shared_ptr<std::ifstream> scenario;
    };
}