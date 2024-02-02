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

        bool has(std::string const &id) const;

        template<typename T>
        void addData(std::string const &id, T *x, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->addData(id, x);
        }

        template<typename T>
        void addData(std::string const &id, T *x) {
            this->removeData(id);
            mapSet(this->data, id, (void *) x);
        }

        template<typename T>
        void addDataPassPointerOwnership(std::string const &id, T *x, bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->template addDataPassPointerOwnership(id, x);
        }

        template<typename T>
        void addDataPassPointerOwnership(std::string const &id, T *x) {
            this->template addDataPassPointerOwnership(id, x, this->template defaultDeleter<T>());
        }

        template<typename T>
        void addDataPassPointerOwnership(std::string const &id, T *x, std::function<void(void *)> const &deleter,
                                         bool errorOnReplace) {
            this->checkErrorOnReplace(id, errorOnReplace);
            this->template addDataPassPointerOwnership(id, x, deleter);
        }

        template<typename T>
        void addDataPassPointerOwnership(std::string const &id, T *x, std::function<void(void *)> const &deleter) {
            this->removeData(id);
            mapSet(this->data, id, (void *) x);
            mapSet(this->deleters, id, deleter);
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
            this->removeData(id);
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
            this->removeData(id);
            mapSet(this->data, id, (void *) new typename std::remove_reference<T>::type(std::forward<T>(x)));
            mapSet(this->deleters, id, deleter);
        }

        [[nodiscard]] void *getData(std::string const &id) const;

        template<typename T>
        T *getData(std::string const &id) const {
            return (T *) this->getData(id);
        }

        bool getDataIfContains(std::string const &id, void *&res) const;

        template<typename T>
        bool getDataIfContains(std::string const &id, T *&res) const {
            void *resTmp = nullptr;
            if (this->getDataIfContains(id, resTmp)) {
                res = (T *) resTmp;
                return true;
            }
            return false;
        }

        template<typename T>
        void updateData(std::string const &id, T const &x) {
            void *dataTmp;
            if (!AndreiUtils::mapGetIfContains(this->data, id, dataTmp)) {
                throw std::runtime_error("Can not update data in container if no data is present (id = " + id + ")");
            }
            *(T *) dataTmp = x;
        }

        std::map<std::string, void *>::iterator begin();

        std::map<std::string, void *>::iterator end();

        [[nodiscard]] std::map<std::string, void *>::const_iterator begin() const;

        [[nodiscard]] std::map<std::string, void *>::const_iterator end() const;

    protected:
        void removeData(std::string const &id) {
            void *datum;
            if (AndreiUtils::mapGetIfContains(this->data, id, datum)) {
                std::function<void(void *)> *deleter;
                if (AndreiUtils::mapGetIfContains(this->deleters, id, deleter)) {
                    (*deleter)(datum);
                    AndreiUtils::mapDelete(this->deleters, id);
                }
                AndreiUtils::mapDelete(this->data, id);
            }
        }

        void checkErrorOnReplace(std::string const &id, bool errorOnReplace) const;

        template<typename T>
        std::function<void(void *)> defaultDeleter() {
            return [](void *_data) {
                delete (typename std::remove_reference<T>::type *) _data;
            };
        }

        std::map<std::string, void *> data;
        std::map<std::string, std::function<void(void *)>> deleters;
    };
}

#endif //ANDREIUTILS_MIXEDDATACONTAINER_HPP
