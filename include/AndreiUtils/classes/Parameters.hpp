//
// Created by Andrei on 18.12.23.
//

#pragma once

#include <AndreiUtils/json.hpp>
#include <string>

namespace AndreiUtils {
    class Parameters {
    public:
        Parameters();

        explicit Parameters(nlohmann::json config);

        explicit Parameters(nlohmann::json *config);

        Parameters(Parameters const &other);

        Parameters(Parameters &&other) noexcept;

        virtual ~Parameters() noexcept;

        Parameters &operator=(Parameters const &other);

        Parameters &operator=(Parameters &&other) noexcept;

        [[nodiscard]] bool operator==(Parameters const &other) const;

        [[nodiscard]] bool operator!=(Parameters const &other) const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] bool has(std::string const &parameterName) const;

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName) const {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not get json value from a nullptr reference!");
                }
                return this->parameterReference->at(parameterName).get<T>();
            }
            return this->parameters.at(parameterName).get<T>();
        }

        template<typename T>
        [[nodiscard]] T get(std::string const &parameterName, T const &defaultValue) const {
            if (this->has(parameterName)) {
                return this->get<T>(parameterName);
            }
            return defaultValue;
        }

        virtual void set(nlohmann::json const &data);

        virtual void set(nlohmann::json &&data);

        template<typename T>
        void set(std::string const &parameterName, T const &data) {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not set json value in a nullptr reference!");
                }
                (*this->parameterReference)[parameterName] = data;
            } else {
                this->parameters[parameterName] = data;
            }
        }

        template<typename T>
        void set(std::string const &parameterName, T &&data) {
            if (this->isReference) {
                if (this->parameterReference == nullptr) {
                    throw std::runtime_error("Can not set json value in a nullptr reference!");
                }
                (*this->parameterReference)[parameterName] = std::forward<T>(data);
            } else {
                this->parameters[parameterName] = std::forward<T>(data);
            }
        }

        virtual bool deleteKey(std::string const &parameterName);

        [[nodiscard]] Parameters operator[](std::string const &parameterName);

        [[nodiscard]] Parameters at(std::string const &parameterName);

        [[nodiscard]] nlohmann::json const &getCreateJson(std::string const &parameterName);

        [[nodiscard]] nlohmann::json const &getJson(std::string const &parameterName) const;

        [[nodiscard]] nlohmann::json const &getJson() const;

        [[nodiscard]] nlohmann::json &getCreateJsonReference(std::string const &parameterName);

        [[nodiscard]] nlohmann::json &getJsonReference(std::string const &parameterName);

        [[nodiscard]] nlohmann::json &getJsonReference();

        virtual void clear();

    protected:
        bool isReference;
        nlohmann::json parameters;
        nlohmann::json *parameterReference;
    };
}
