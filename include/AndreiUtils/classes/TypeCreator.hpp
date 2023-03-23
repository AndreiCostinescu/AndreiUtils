//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_TYPECREATOR_HPP
#define ANDREIUTILS_TYPECREATOR_HPP

#include <AndreiUtils/utilsMap.hpp>
#include <memory>

namespace AndreiUtils {
    template<class TypeID, class Type, class CreatorArgumentsType>
    class ConfigurableUniqueTypeCreatorWithID {
    public:
        ConfigurableUniqueTypeCreatorWithID() = default;

        virtual ~ConfigurableUniqueTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(
                TypeID const &typeId, std::function<std::unique_ptr<Type>(CreatorArgumentsType)> typeCreator,
                bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        virtual void registerTypeCreator(TypeID const &typeId,
                                         std::function<std::unique_ptr<Type>(CreatorArgumentsType)> typeCreator) {
            this->typeCreators[typeId] = std::move(typeCreator);
        }

        virtual void mergeTypeCreators(
                ConfigurableUniqueTypeCreatorWithID<TypeID, Type, CreatorArgumentsType> const &other,
                bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual std::unique_ptr<Type> createType(TypeID const &typeId, CreatorArgumentsType config) const {
            return mapGet(this->typeCreators, typeId)(config);
        }

        virtual std::map<TypeID, std::function<std::unique_ptr<Type>(CreatorArgumentsType)>> const &
        getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<std::unique_ptr<Type>(CreatorArgumentsType)>> &getRegisteredTypes() {
            return this->typeCreators;
        }

        [[nodiscard]] bool isTypeRegistered(TypeID const &type) const {
            return AndreiUtils::mapContains(this->typeCreators, type);
        }

    protected:
        std::map<TypeID, std::function<std::unique_ptr<Type>(CreatorArgumentsType)>> typeCreators;
    };

    template<class Type, class CreatorArgumentsType>
    class ConfigurableUniqueTypeCreator
            : public ConfigurableUniqueTypeCreatorWithID<std::string, Type, CreatorArgumentsType> {
    public:
        ConfigurableUniqueTypeCreator() = default;

        virtual ~ConfigurableUniqueTypeCreator() = default;

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::unique_ptr<Type>(CreatorArgumentsType)> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::unique_ptr<Type>(CreatorArgumentsType)> typeCreator) override {
            ConfigurableUniqueTypeCreatorWithID<std::string, Type, CreatorArgumentsType>::registerTypeCreator(
                    typeId, std::move(typeCreator));
        }
    };

    template<class TypeID, class Type>
    class ConfigurableUniqueTypeCreatorWithID<TypeID, Type, void> {
    public:
        ConfigurableUniqueTypeCreatorWithID() = default;

        virtual ~ConfigurableUniqueTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(TypeID const &typeId, std::function<std::unique_ptr<Type>()> typeCreator,
                                         bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        virtual void registerTypeCreator(TypeID const &typeId, std::function<std::unique_ptr<Type>()> typeCreator) {
            this->typeCreators[typeId] = std::move(typeCreator);
        }

        virtual void mergeTypeCreators(ConfigurableUniqueTypeCreatorWithID<TypeID, Type, void> const &other,
                                       bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual std::unique_ptr<Type> createType(TypeID const &typeId) const {
            return mapGet(this->typeCreators, typeId)();
        }

        virtual std::map<TypeID, std::function<std::unique_ptr<Type>()>> const &getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<std::unique_ptr<Type>()>> &getRegisteredTypes() {
            return this->typeCreators;
        }

    protected:
        std::map<TypeID, std::function<std::unique_ptr<Type>()>> typeCreators;
    };

    template<class Type>
    class ConfigurableUniqueTypeCreator<Type, void>
            : public ConfigurableUniqueTypeCreatorWithID<std::string, Type, void> {
    public:
        ConfigurableUniqueTypeCreator() = default;

        virtual ~ConfigurableUniqueTypeCreator() = default;

        void registerTypeCreator(std::string const &typeId, std::function<std::unique_ptr<Type>()> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::unique_ptr<Type>()> typeCreator) override {
            ConfigurableUniqueTypeCreatorWithID<std::string, Type, void>::registerTypeCreator(typeId,
                                                                                              std::move(typeCreator));
        }
    };


    template<class TypeID, class Type, class CreatorArgumentsType>
    class ConfigurableSharedTypeCreatorWithID {
    public:
        ConfigurableSharedTypeCreatorWithID() = default;

        virtual ~ConfigurableSharedTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(
                TypeID const &typeId, std::function<std::shared_ptr<Type>(CreatorArgumentsType)> typeCreator,
                bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        virtual void registerTypeCreator(TypeID const &typeId,
                                         std::function<std::shared_ptr<Type>(CreatorArgumentsType)> typeCreator) {
            this->typeCreators[typeId] = std::move(typeCreator);
        }

        virtual void mergeTypeCreators(
                ConfigurableSharedTypeCreatorWithID<TypeID, Type, CreatorArgumentsType> const &other,
                bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual std::shared_ptr<Type> createType(TypeID const &typeId, CreatorArgumentsType config) const {
            return mapGet(this->typeCreators, typeId)(config);
        }

        virtual std::map<TypeID, std::function<std::shared_ptr<Type>(CreatorArgumentsType)>> const &
        getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<std::shared_ptr<Type>(CreatorArgumentsType)>> &getRegisteredTypes() {
            return this->typeCreators;
        }

        [[nodiscard]] bool isTypeRegistered(TypeID const &type) const {
            return AndreiUtils::mapContains(this->typeCreators, type);
        }

    protected:
        std::map<TypeID, std::function<std::shared_ptr<Type>(CreatorArgumentsType)>> typeCreators;
    };

    template<class Type, class CreatorArgumentsType>
    class ConfigurableSharedTypeCreator
            : public ConfigurableSharedTypeCreatorWithID<std::string, Type, CreatorArgumentsType> {
    public:
        ConfigurableSharedTypeCreator() = default;

        virtual ~ConfigurableSharedTypeCreator() = default;

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::shared_ptr<Type>(CreatorArgumentsType)> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::shared_ptr<Type>(CreatorArgumentsType)> typeCreator) override {
            ConfigurableSharedTypeCreatorWithID<std::string, Type, CreatorArgumentsType>::registerTypeCreator(
                    typeId, std::move(typeCreator));
        }
    };

    template<class TypeID, class Type>
    class ConfigurableSharedTypeCreatorWithID<TypeID, Type, void> {
    public:
        ConfigurableSharedTypeCreatorWithID() = default;

        virtual ~ConfigurableSharedTypeCreatorWithID() {
            this->typeCreators.clear();
        };

        virtual void registerTypeCreator(TypeID const &typeId, std::function<std::shared_ptr<Type>()> typeCreator,
                                         bool errorOnReplace) {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Given typeID is already a registered type! Not replacing!");
            }
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        virtual void registerTypeCreator(TypeID const &typeId, std::function<std::shared_ptr<Type>()> typeCreator) {
            this->typeCreators[typeId] = std::move(typeCreator);
        }

        virtual void mergeTypeCreators(ConfigurableSharedTypeCreatorWithID<TypeID, Type, void> const &other,
                                       bool withOverwrite) {
            for (const auto &otherTypeCreator: other.getRegisteredTypes()) {
                if (withOverwrite || !mapContains(this->typeCreators, otherTypeCreator.first)) {
                    this->typeCreators[otherTypeCreator.first] = otherTypeCreator.second;
                }
            }
        }

        virtual std::shared_ptr<Type> createType(TypeID const &typeId) const {
            return mapGet(this->typeCreators, typeId)();
        }

        virtual std::map<TypeID, std::function<std::shared_ptr<Type>()>> const &getRegisteredTypes() const {
            return this->typeCreators;
        }

        virtual std::map<TypeID, std::function<std::shared_ptr<Type>()>> &getRegisteredTypes() {
            return this->typeCreators;
        }

    protected:
        std::map<TypeID, std::function<std::shared_ptr<Type>()>> typeCreators;
    };

    template<class Type>
    class ConfigurableSharedTypeCreator<Type, void>
            : public ConfigurableSharedTypeCreatorWithID<std::string, Type, void> {
    public:
        ConfigurableSharedTypeCreator() = default;

        virtual ~ConfigurableSharedTypeCreator() = default;

        void registerTypeCreator(std::string const &typeId, std::function<std::shared_ptr<Type>()> typeCreator,
                                 bool errorOnReplace) override {
            if (mapContains(this->typeCreators, typeId) && errorOnReplace) {
                throw std::runtime_error("Type " + typeId + " is already a registered type! Not replacing!");
            }
            // this->template registerTypeCreator<std::string, Type>(typeId, typeCreator);
            this->registerTypeCreator(typeId, std::move(typeCreator));
        }

        void registerTypeCreator(std::string const &typeId,
                                 std::function<std::shared_ptr<Type>()> typeCreator) override {
            ConfigurableSharedTypeCreatorWithID<std::string, Type, void>::registerTypeCreator(typeId,
                                                                                              std::move(typeCreator));
        }
    };


    template<class TypeID, class Type>
    using SharedTypeCreatorWithID = ConfigurableSharedTypeCreatorWithID<TypeID, Type, void>;

    template<class Type>
    using SharedTypeCreator = ConfigurableSharedTypeCreator<Type, void>;

    template<class TypeID, class Type>
    using UniqueTypeCreatorWithID = ConfigurableUniqueTypeCreatorWithID<TypeID, Type, void>;

    template<class Type>
    using UniqueTypeCreator = ConfigurableUniqueTypeCreator<Type, void>;
}

#endif //ANDREIUTILS_TYPECREATOR_HPP
