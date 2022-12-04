//
// Created by Andrei on 30-Mar-22.
//

#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/utilsMap.hpp>

using namespace AndreiUtils;
using namespace std;

MixedDataContainer::MixedDataContainer() : data() {}

MixedDataContainer::MixedDataContainer(MixedDataContainer &&other) noexcept:
        data(std::move(other.data)), deleters(std::move(other.deleters)) {}

MixedDataContainer &MixedDataContainer::operator=(MixedDataContainer &&other) noexcept {
    if (this != &other) {
        this->data = std::move(other.data);
        this->deleters = std::move(other.deleters);
    }
    return *this;
}

MixedDataContainer::~MixedDataContainer() {
    std::function<void(void *)> deleter;
    for (auto const &name: this->data) {
        if (mapGetIfContains(this->deleters, name.first, deleter)) {
            deleter(name.second);
        }
    }
}

bool MixedDataContainer::has(string const &id) const {
    return AndreiUtils::mapContains(this->data, id);
}

void *MixedDataContainer::getData(string const &id) const {
    void *res = nullptr;
    if (!mapGetIfContains<std::string, void *>(this->data, id, res)) {
        throw std::runtime_error("Data " + id + " not registered in container!");
    }
    return res;
}

bool MixedDataContainer::getDataIfContains(string const &id, void *&res) const {
    return mapGetIfContains<std::string, void *>(this->data, id, res);
}

std::map<std::string, void *>::iterator MixedDataContainer::begin() {
    return this->data.begin();
}

std::map<std::string, void *>::iterator MixedDataContainer::end() {
    return this->data.end();
}

std::map<std::string, void *>::const_iterator MixedDataContainer::begin() const {
    return this->data.begin();
}

std::map<std::string, void *>::const_iterator MixedDataContainer::end() const {
    return this->data.end();
}

void MixedDataContainer::checkErrorOnReplace(string const &id, bool errorOnReplace) const {
    if (errorOnReplace && mapContains(this->data, id)) {
        throw std::runtime_error("Data " + id + " is already in the container! Not replacing!");
    }
}
