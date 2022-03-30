//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_TYPECREATOR_HPP
#define ANDREIUTILS_TYPECREATOR_HPP

#include <AndreiUtils/json.hpp>
#include <AndreiUtils/utilsMap.hpp>

namespace AndreiUtils {
    template<class TypeID, class Type>
    class TypeCreatorWithID {
    public:
        TypeCreatorWithID() = default;

        virtual ~TypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(const TypeID &typeId, std::function<Type *()> typeCreator,
                                         bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, typeCreator);
        }

        virtual void registerTypeCreator(const TypeID &typeId, std::function<Type *()> typeCreator) {
            this->typeCreators[typeId] = typeCreator;
        }

        virtual void mergeTypeCreators(const TypeCreatorWithID<TypeID, Type> &other, bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual Type *createType(const TypeID &typeId) const {
            return mapGet(this->typeCreators, typeId)();
        }

        virtual std::map<TypeID, std::function<Type *()>> getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<Type *()>> &getRegisteredTypes() {
            return this->typeCreators;
        }

    protected:
        std::map<TypeID, std::function<Type *()>> typeCreators;
    };

    template<class Type>
    class TypeCreator : public TypeCreatorWithID<std::string, Type> {
    public:
        TypeCreator() = default;

        virtual ~TypeCreator() = default;

        void registerTypeCreator(const std::string &typeId, std::function<Type *()> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, typeCreator);
        }

        void registerTypeCreator(const std::string &typeId, std::function<Type *()> typeCreator) override {
            TypeCreatorWithID<std::string, Type>::registerTypeCreator(typeId, typeCreator);
        }
    };

    template<class TypeID, class Type>
    class ConfigurableTypeCreatorWithID {
    public:
        ConfigurableTypeCreatorWithID() = default;

        virtual ~ConfigurableTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(const TypeID &typeId,
                                         std::function<Type *(const nlohmann::json &)> typeCreator,
                                         bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        virtual void registerTypeCreator(const TypeID &typeId,
                                         std::function<Type *(const nlohmann::json &json)> typeCreator) {
            this->typeCreators[typeId] = move(typeCreator);
        }

        virtual void mergeTypeCreators(const ConfigurableTypeCreatorWithID<TypeID, Type> &other, bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual Type *createType(const TypeID &typeId, const nlohmann::json &config) const {
            return mapGet(this->typeCreators, typeId)(config);
        }

        virtual std::map<TypeID, std::function<Type *(const nlohmann::json &)>> getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<Type *(const nlohmann::json &)>> &getRegisteredTypes() {
            return this->typeCreators;
        }

    protected:
        std::map<TypeID, std::function<Type *(const nlohmann::json &)>> typeCreators;
    };

    template<class Type>
    class ConfigurableTypeCreator : public ConfigurableTypeCreatorWithID<std::string, Type> {
    public:
        ConfigurableTypeCreator() = default;

        virtual ~ConfigurableTypeCreator() = default;

        void registerTypeCreator(const std::string &typeId, std::function<Type *(const nlohmann::json &)> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        void registerTypeCreator(const std::string &typeId,
                                 std::function<Type *(const nlohmann::json &)> typeCreator) override {
            ConfigurableTypeCreatorWithID<std::string, Type>::registerTypeCreator(typeId, move(typeCreator));
        }
    };
}

#endif //ANDREIUTILS_TYPECREATOR_HPP
