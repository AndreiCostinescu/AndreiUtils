//
// Created by Andrei on 21-Jan-22.
//

#ifndef ANDREIUTILS_TYPECREATOR_HPP
#define ANDREIUTILS_TYPECREATOR_HPP

#include <AndreiUtils/utils.hpp>
#include <functional>
#include <map>
#include <string>

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
                std::function<Type *()> typeCreator;
                if (!mapGetIfContains(this->typeCreators, otherTypeCreator.first, typeCreator) || withOverwrite) {
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
}

#endif //ANDREIUTILS_TYPECREATOR_HPP
