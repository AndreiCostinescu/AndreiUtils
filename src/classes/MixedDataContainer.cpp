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

void MixedDataContainer::checkErrorOnReplace(string const &id, bool errorOnReplace) const {
    if (errorOnReplace && mapContains(this->data, id)) {
        throw std::runtime_error("Data " + id + " is already in the container! Not replacing!");
    }
}
