//
// Created by Andrei on 30-Mar-22.
//

#ifndef ANDREIUTILS_MIXEDDATACONTAINER_HPP
#define ANDREIUTILS_MIXEDDATACONTAINER_HPP

#include <AndreiUtils/utilsMap.hpp>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

namespace AndreiUtils {
    class MixedDataContainer {
    public:
        MixedDataContainer();

        MixedDataContainer(MixedDataContainer const &other) = delete;

        MixedDataContainer(MixedDataContainer &&other) noexcept;

        MixedDataContainer &operator=(MixedDataContainer const &other) = delete;

        MixedDataContainer &operator=(MixedDataContainer &&other) noexcept;

        virtual ~MixedDataContainer();
\
        template<typename T>
        void addData(std::string const &id, T *x, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, x);
        }

        template<typename T>
        void addData(std::string const &id, T *x) {
            mapSet(this->data, id, (void *) x);
        }

        template<typename T>
        void addData(std::string const &id, T const &x, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, x);
        }

        template<typename T>
        void addData(std::string const &id, T const &x) {
            this->template addData(id, x, this->template defaultDeleter<T>());
        }

        template<typename T>
        void addData(std::string const &id, T const &x, std::function<void(void *)> const &deleter,
                     bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, x, deleter);
        }

        template<typename T>
        void addData(std::string const &id, T const &x, std::function<void(void *)> const &deleter) {
            mapSet(this->data, id, (void *) new T(x));
            mapSet(this->deleters, id, deleter);
        }

        template<typename T>
        void addData(std::string const &id, T &&x, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, std::forward<T>(x));
        }

        template<typename T>
        void addData(std::string const &id, T &&x) {
            this->template addData(id, std::forward<T>(x), this->template defaultDeleter<T>());
        }

        template<typename T>
        void addData(std::string const &id, T &&x, std::function<void(void *)> const &deleter, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, std::forward<T>(x), deleter);
        }

        template<typename T>
        void addData(std::string const &id, T &&x, std::function<void(void *)> const &deleter) {
            mapSet(this->data, id, (void *) new typename std::remove_reference<T>::type(std::forward<T>(x)));
            mapSet(this->deleters, id, deleter);
        }

        template<typename T>
        T *getData(std::string const &id) const {
            void *res = nullptr;
            if (!mapGetIfContains<std::string, void *>(this->data, id, res)) {
                throw std::runtime_error("Data " + id + " not registered in container!");
            }
            return (T *) res;
        }

    protected:
        void checkErrorOnReplace(std::string const &id, bool errorOnReplace) const;

        template<typename T>
        std::function<void(void *)> defaultDeleter() {
            return [](void *_data) {
                std::cout << "Enter default deleter " << std::endl;
                delete (typename std::remove_reference<T>::type *) _data;
            };
        }

        std::map<std::string, void *> data;
        std::map<std::string, std::function<void(void *)>> deleters;
    };
}

#endif //ANDREIUTILS_MIXEDDATACONTAINER_HPP
