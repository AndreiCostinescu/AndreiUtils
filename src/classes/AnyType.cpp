//
// Created by Andrei on 07.12.22.
//

#include <AndreiUtils/classes/AnyType.h>

using namespace AndreiUtils;
using namespace std;

AnyType::AnyType() : content(nullptr) {}

AnyType::AnyType(AnyType const &other) : content(!other.empty() ? other.content->clone() : nullptr) {}

AnyType::AnyType(AnyType &&other) noexcept: content(other.content) {
    other.content = nullptr;
}

AnyType::~AnyType() {
    delete this->content;
}

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

bool AnyType::empty() const {
    return this->content == nullptr;
}

std::type_info const &AnyType::getType() const {
    return this->content ? this->content->getType() : typeid(void);
}

std::string AnyType::getTypeName() const {
    return this->content ? this->content->getTypeName() : typeid(void).name();
}

AnyType::PlaceHolder::~PlaceHolder() = default;
