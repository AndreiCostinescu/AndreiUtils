//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_TYPECREATOR_HPP
#define ANDREIUTILS_TYPECREATOR_HPP

#include <AndreiUtils/utilsMap.hpp>

namespace AndreiUtils {
    template<class TypeID, class Type, class CreatorArgumentsType>
    class ConfigurableTypeCreatorWithID {
    public:
        ConfigurableTypeCreatorWithID() = default;

        virtual ~ConfigurableTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(
                const TypeID &typeId, std::function<Type *(CreatorArgumentsType)> typeCreator, bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        virtual void registerTypeCreator(const TypeID &typeId,
                                         std::function<Type *(CreatorArgumentsType)> typeCreator) {
            this->typeCreators[typeId] = move(typeCreator);
        }

        virtual void mergeTypeCreators(const ConfigurableTypeCreatorWithID<TypeID, Type, CreatorArgumentsType> &other,
                                       bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual Type *createType(const TypeID &typeId, CreatorArgumentsType config) const {
            return mapGet(this->typeCreators, typeId)(config);
        }

        virtual std::map<TypeID, std::function<Type *(CreatorArgumentsType)>> getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<Type *(CreatorArgumentsType)>> &getRegisteredTypes() {
            return this->typeCreators;
        }

    protected:
        std::map<TypeID, std::function<Type *(CreatorArgumentsType)>> typeCreators;
    };

    template<class Type, class CreatorArgumentsType>
    class ConfigurableTypeCreator : public ConfigurableTypeCreatorWithID<std::string, Type, CreatorArgumentsType> {
    public:
        ConfigurableTypeCreator() = default;

        virtual ~ConfigurableTypeCreator() = default;

        void registerTypeCreator(const std::string &typeId, std::function<Type *(CreatorArgumentsType)> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        void registerTypeCreator(const std::string &typeId,
                                 std::function<Type *(CreatorArgumentsType)> typeCreator) override {
            ConfigurableTypeCreatorWithID<std::string, Type, CreatorArgumentsType>::registerTypeCreator(
                    typeId, move(typeCreator));
        }
    };

    template<class TypeID, class Type>
    class ConfigurableTypeCreatorWithID<TypeID, Type, void> {
    public:
        ConfigurableTypeCreatorWithID() = default;

        virtual ~ConfigurableTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(const TypeID &typeId, std::function<Type *()> typeCreator,
                                         bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        virtual void registerTypeCreator(const TypeID &typeId, std::function<Type *()> typeCreator) {
            this->typeCreators[typeId] = move(typeCreator);
        }

        virtual void mergeTypeCreators(const ConfigurableTypeCreatorWithID<TypeID, Type, void> &other,
                                       bool withOverwrite) {
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
    class ConfigurableTypeCreator<Type, void> : public ConfigurableTypeCreatorWithID<std::string, Type, void> {
    public:
        ConfigurableTypeCreator() = default;

        virtual ~ConfigurableTypeCreator() = default;

        void registerTypeCreator(const std::string &typeId, std::function<Type *()> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, move(typeCreator));
        }

        void registerTypeCreator(const std::string &typeId, std::function<Type *()> typeCreator) override {
            ConfigurableTypeCreatorWithID<std::string, Type, void>::registerTypeCreator(typeId, move(typeCreator));
        }
    };

    template<class TypeID, class Type>
    using TypeCreatorWithID = ConfigurableTypeCreatorWithID<TypeID, Type, void>;

    template<class Type>
    using TypeCreator = ConfigurableTypeCreator<Type, void>;
}

#endif //ANDREIUTILS_TYPECREATOR_HPP
