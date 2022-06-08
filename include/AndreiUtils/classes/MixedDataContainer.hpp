//
// Created by Andrei on 30-Mar-22.
//

#ifndef ANDREIUTILS_MIXEDDATACONTAINER_HPP
#define ANDREIUTILS_MIXEDDATACONTAINER_HPP

#include <AndreiUtils/utilsMap.hpp>
#include <map>
#include <string>
#include <utility>

namespace AndreiUtils {
    class MixedDataContainer {
    public:
        MixedDataContainer();

        virtual ~MixedDataContainer();

        template<typename T>
        void addData(const std::string &id, T *x, bool errorOnReplace) {
            if (mapContains(this->data, id) && errorOnReplace) {
                throw std::runtime_error("Data " + id + " is already in the container! Not replacing!");
            }
            this->addData(id, x);
        }

        template<typename T>
        void addData(const std::string &id, T *x) {
            mapSet(this->data, id, (void *) x);
        }

        template<typename T>
        T *getData(const std::string &id) const {
            void *res = nullptr;
            void *const *resPtr = &res;
            if (!mapGetIfContains<std::string, void *>(this->data, id, resPtr)) {
                throw std::runtime_error("Data " + id + " not registered in container!");
            }
            return (T *) res;
        }

    protected:
        std::map<std::string, void *> data;
    };
}

#endif //ANDREIUTILS_MIXEDDATACONTAINER_HPP
