//
// Created by Andrei on 07.12.22.
//

#include <AndreiUtils/classes/AnyType.h>
#include <iostream>

using namespace AndreiUtils;
using namespace std;

AnyType::AnyType() : content(nullptr) {}

AnyType::AnyType(AnyType const &other) : content(!other.empty() ? other.content->clone() : nullptr) {}

AnyType::AnyType(AnyType &&other) noexcept: content(other.content) {
    other.content = nullptr;
}

AnyType::~AnyType() {
    cout << "In destructor!" << endl;
    delete content;
}

AnyType &AnyType::swap(AnyType &other) {
    std::swap(content, other.content);
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

bool AnyType::empty() const {
    return this->content == nullptr;
}

std::type_info const &AnyType::getType() const {
    return content ? content->getType() : typeid(void);
}

AnyType::PlaceHolder::~PlaceHolder() = default;
