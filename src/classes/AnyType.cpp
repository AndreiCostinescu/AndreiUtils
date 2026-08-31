/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 07.12.22.
//

#include <AndreiUtils/classes/AnyType.h>

using namespace AndreiUtils;
using namespace std;

AnyType::AnyType() : content(nullptr) {}

AnyType::AnyType(AnyType const &other) : content(!other.empty() ? other.content->clone() : nullptr) {}

AnyType::AnyType(AnyType &&other) noexcept : content(other.content) { other.content = nullptr; }

AnyType::~AnyType() { delete this->content; }

AnyType &AnyType::swap(AnyType &other) {
    std::swap(this->content, other.content);
    return *this;
}

AnyType &AnyType::operator=(AnyType const &other) {
    if (this != &other) {
        AnyType(other).swap(*this);
    }
    return *this;
}

AnyType &AnyType::operator=(AnyType &&other) noexcept {
    if (this != &other) {
        AnyType(std::move(other)).swap(*this);
    }
    return *this;
}

bool AnyType::empty() const { return this->content == nullptr; }

std::type_info const &AnyType::getType() const { return this->content ? this->content->getType() : typeid(void); }

std::string AnyType::getTypeName() const { return this->content ? this->content->getTypeName() : typeid(void).name(); }

AnyType::PlaceHolder::~PlaceHolder() = default;
