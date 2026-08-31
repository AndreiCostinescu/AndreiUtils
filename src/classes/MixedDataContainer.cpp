// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 30-Mar-22.
//

#include <AndreiUtils/classes/MixedDataContainer.hpp>
#include <AndreiUtils/utilsMap.hpp>
#include <cassert>

using namespace AndreiUtils;
using namespace std;

MixedDataContainer::MixedDataContainer() : data() {}

MixedDataContainer::MixedDataContainer(MixedDataContainer &&other) noexcept :
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

bool MixedDataContainer::has(string const &id) const { return AndreiUtils::mapContains(this->data, id); }

void *MixedDataContainer::getData(string const &id) const {
    void *const *res = nullptr;
    if (!mapGetIfContains<std::string, void *>(this->data, id, res)) {
        throw std::runtime_error("Data " + id + " not registered in container!");
    }
    assert(res != nullptr);
    return *res;
}

bool MixedDataContainer::getDataIfContains(string const &id, void *&res) const {
    return mapGetIfContains<std::string, void *>(this->data, id, res);
}

std::map<std::string, void *>::iterator MixedDataContainer::begin() { return this->data.begin(); }

std::map<std::string, void *>::iterator MixedDataContainer::end() { return this->data.end(); }

std::map<std::string, void *>::const_iterator MixedDataContainer::begin() const { return this->data.begin(); }

std::map<std::string, void *>::const_iterator MixedDataContainer::end() const { return this->data.end(); }

void MixedDataContainer::checkErrorOnReplace(string const &id, bool errorOnReplace) const {
    if (errorOnReplace && mapContains(this->data, id)) {
        throw std::runtime_error("Data " + id + " is already in the container! Not replacing!");
    }
}
