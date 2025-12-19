//
// Created by Andrei on 03.11.24.
//

#include <AndreiUtils/classes/Pointer.hpp>

namespace AndreiUtils {
    template<typename T>
    Pointer<T>::Pointer() : isRegular(true), regular(nullptr), smart(nullptr) {}

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T>::Pointer(std::nullptr_t) : Pointer() {}

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T>::Pointer(Type &datum) : isRegular(true), regular(&datum), smart() {}

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T>::Pointer(Type const &datum) : Pointer() {
        if constexpr (std::is_const_v<T>) {
            this->regular = &datum;
        } else {
            this->smart = std::make_shared<Type>(datum);
            this->isRegular = false;
        }
    }

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T>::Pointer(Type &&datum) : isRegular(false), regular(nullptr),
                                        smart(std::make_shared<Type>(std::forward<Type>(datum))) {}

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T>::Pointer(T *datum) : isRegular(true), regular(datum), smart(nullptr) {}

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T>::Pointer(std::remove_const_t<T> *datum) requires std::is_const_v<T> :
        isRegular(true), regular(datum), smart(nullptr) {}

    // no marking as explicit because we want the conversion from SmartPtrType to Pointer
    template<typename T>
    Pointer<T>::Pointer(SmartPtrType datum) : isRegular(false), regular(nullptr), smart(std::move(datum)) {}

    // no marking as explicit because we want the conversion from SmartPtrType to Pointer
    template<typename T>
    template<typename SubT> requires StrictSubType<T, SubT>
    Pointer<T>::Pointer(std::shared_ptr<SubT> datum) : isRegular(false), regular(nullptr), smart(std::move(datum)) {}

    template<typename T>
    Pointer<T>::Pointer(Pointer const &other) : isRegular(other.isRegular), regular(other.regular),
                                                smart(other.smart) {}

    template<typename T>
    Pointer<T>::Pointer(Pointer &&other) noexcept : isRegular(other.isRegular), regular(other.regular),
                                                    smart(std::move(other.smart)) {
        other.regular = nullptr;
        other.isRegular = false;
    }

    template<typename T>
    Pointer<T>::Pointer(Pointer<std::remove_const_t<T>> const &other) requires std::is_const_v<T> :
        Pointer(other.constCast()) {}

    template<typename T>
    Pointer<T>::Pointer(Pointer<std::remove_const_t<T>> &&other) noexcept requires std::is_const_v<T> :
        Pointer(std::move(std::move(other).constCastMove())) {}

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && TypesWithSameConst<T, Type>
    Pointer<T>::Pointer(Pointer<Type> const &other) : Pointer(other.template cast<T>()) {}

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && TypesWithSameConst<T, Type>
    Pointer<T>::Pointer(Pointer<Type> &&other) noexcept : Pointer(std::move(std::move(other).template castMove<T>())) {}

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && FirstConstSecondNot<T, Type>
    Pointer<T>::Pointer(Pointer<Type> const &other) : Pointer(other.constCast()) {}

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && FirstConstSecondNot<T, Type>
    Pointer<T>::Pointer(Pointer<Type> &&other) noexcept : Pointer(std::move(std::move(other).constCastMove())) {}

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T> &Pointer<T>::operator=(Type &other) {
        this->isRegular = true;
        this->regular = &other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T> &Pointer<T>::operator=(Type const &other) {
        if constexpr (std::is_const_v<T>) {
            this->regular = &other;
            this->isRegular = true;
        } else {
            this->smart = std::make_shared<Type>(other);
            this->isRegular = false;
        }
        return *this;
    }

    template<typename T>
    template<typename Type> requires SubType<T, Type>
    Pointer<T> &Pointer<T>::operator=(Type &&other) {
        this->isRegular = false;
        this->regular = nullptr;
        this->smart = std::make_shared<std::conditional_t<std::is_const_v<T>, Type const, Type>>(
                std::forward<Type>(other));
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(std::nullptr_t) {
        this->isRegular = true;
        this->regular = nullptr;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(T *other) {
        this->isRegular = true;
        this->regular = other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(std::remove_const_t<T> *other) requires std::is_const_v<T> {
        this->isRegular = true;
        this->regular = other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(SmartPtrType other) {
        this->isRegular = false;
        this->regular = nullptr;
        this->smart = std::move(other);
        return *this;
    }

    template<typename T>
    template<typename SubT> requires StrictSubType<T, SubT>
    Pointer<T> &Pointer<T>::operator=(std::shared_ptr<SubT> other) {
        this->isRegular = false;
        this->regular = nullptr;
        this->smart = std::move(other);
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(Pointer const &other) {
        if (this != &other) {
            this->isRegular = other.isRegular;
            this->regular = other.regular;
            this->smart = other.smart;
        }
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(Pointer &&other) noexcept {
        if (this != &other) {
            this->isRegular = other.isRegular;
            this->regular = other.regular;
            this->smart = std::move(other.smart);
            other.regular = nullptr;
            other.isRegular = false;
        }
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(Pointer<std::remove_const_t<T>> const &other) requires std::is_const_v<T> {
        *this = other.constCast();
        return *this;
    }

    template<typename T>
    Pointer<T> &Pointer<T>::operator=(Pointer<std::remove_const_t<T>> &&other) noexcept requires std::is_const_v<T> {
        *this = std::move(std::move(other).constCastMove());
        return *this;
    }

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && TypesWithSameConst<T, Type>
    Pointer<T> &Pointer<T>::operator=(Pointer<Type> const &other) {
        *this = other.template cast<T>();
        return *this;
    }

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && TypesWithSameConst<T, Type>
    Pointer<T> &Pointer<T>::operator=(Pointer<Type> &&other) noexcept {
        *this = std::move(std::move(other).template castMove<T>());
        return *this;
    }

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && FirstConstSecondNot<T, Type>
    Pointer<T> &Pointer<T>::operator=(Pointer<Type> const &other) {
        *this = other.constCast();
        return *this;
    }

    template<typename T>
    template<typename Type> requires StrictSubType<T, Type> && FirstConstSecondNot<T, Type>
    Pointer<T> &Pointer<T>::operator=(Pointer<Type> &&other) noexcept {
        *this = std::move(std::move(other).constCastMove());
        return *this;
    }

    template<typename T>
    bool Pointer<T>::operator<(Pointer<std::remove_const_t<T>> const &other) const {
        return this->get() < other.get();
    }

    template<typename T>
    bool Pointer<T>::operator<(Pointer<std::remove_const_t<T> const> const &other) const {
        return this->get() < other.get();
    }

    template<typename T>
    Pointer<T>::operator bool() const { return this->get() != nullptr; }

    template<typename T>
    T *Pointer<T>::get() const & noexcept {
        return isRegular ? this->regular : this->smart.get();
    }

    template<typename T>
    T *Pointer<T>::operator->() const & noexcept {
        return this->get();
    }

    template<typename T>
    T &Pointer<T>::operator*() const & noexcept {
        return *(this->get());
    }

    template<typename T>
    T *Pointer<T>::ptrOfTempValue() const && noexcept {
        return isRegular ? this->regular : this->smart.get();
    }

    template<typename T>
    T &&Pointer<T>::moveTempValue() const && noexcept {
        return isRegular ? std::move(*this->regular) : std::move(*this->smart);
    }

    template<typename T>
    void Pointer<T>::reset() {
        this->isRegular = true;
        this->regular = nullptr;
        this->smart = nullptr;
    }

    template<typename T>
    Pointer<T const> Pointer<T>::constCast() const noexcept requires NotConst<T> {
        if (this->isRegular) {
            return const_cast<T const *>(this->regular);
        }
        return std::const_pointer_cast<T const>(this->smart);
    }

    template<typename T>
    Pointer<T const> Pointer<T>::constCastMove() && noexcept requires NotConst<T> {
        if (this->isRegular) {
            Pointer<T const> res(const_cast<T const *>(this->regular));
            this->regular = nullptr;
            return res;
        }
        Pointer<T const> res(std::make_shared<T const>(std::move(*this->smart)));
        this->smart = nullptr;
        this->isRegular = true;
        return res;
    }

    template<typename T>
    Pointer<std::remove_const_t<T>> Pointer<T>::constCast() const noexcept requires std::is_const_v<T> {
        if (this->isRegular) {
            return Pointer<std::remove_const_t<T>>(const_cast<std::remove_const_t<T> *>(this->regular));
        }
        return Pointer<std::remove_const_t<T>>(std::const_pointer_cast<std::remove_const_t<T>>(this->smart));
    }

    template<typename T>
    Pointer<std::remove_const_t<T>> Pointer<T>::constCastMove() && noexcept requires std::is_const_v<T> {
        if (this->isRegular) {
            Pointer<std::remove_const_t<T>> res(const_cast<std::remove_const_t<T> *>(this->regular));
            this->regular = nullptr;
            return res;
        }
        Pointer<std::remove_const_t<T>> res(std::make_shared<std::remove_const_t<T>>(std::move(*this->smart)));
        this->smart = nullptr;
        this->isRegular = true;
        return res;
    }

    template<typename T>
    template<typename ParentCastT> requires SubType<ParentCastT, T> && TypesWithSameConst<ParentCastT, T>
    Pointer<ParentCastT> Pointer<T>::cast() const noexcept {
        if (this->isRegular) {
            return Pointer<ParentCastT>(this->regular);
        }
        return Pointer<ParentCastT>(this->smart);
    }

    template<typename T>
    template<typename ParentCastT> requires SubType<ParentCastT, T> && TypesWithSameConst<ParentCastT, T>
    Pointer<ParentCastT> Pointer<T>::castMove() && noexcept {
        if (this->isRegular) {
            Pointer<ParentCastT> res(this->regular);
            this->regular = nullptr;
            return res;
        }
        Pointer<ParentCastT> res(std::move(this->smart));
        this->isRegular = true;
        return res;
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T>::dynamicCast() const noexcept {
        if (this->isRegular) {
            return dynamic_cast<CastT *>(this->regular);
        }
        return std::dynamic_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T>::staticCast() const noexcept {
        if (this->isRegular) {
            return static_cast<CastT *>(this->regular);
        }
        return std::static_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T>::reinterpretCast() const noexcept {
        if (this->isRegular) {
            return reinterpret_cast<CastT *>(this->regular);
        }
        return std::reinterpret_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename TypeCheck>
    bool Pointer<T>::isInstanceOf() const {
        if (this->isRegular) {
            return InstanceOf<TypeCheck, T *,
                              std::is_polymorphic_v<std::remove_pointer_t<T>>>::get(this->regular);
        }
        return InstanceOf<TypeCheck, T *,
                          std::is_polymorphic_v<std::remove_pointer_t<T>>>::get(this->smart.get());
    }

    template<typename T>
    template<typename TypeCheck>
    bool Pointer<T>::isInstanceOf(TypeCheck const *&res) const {
        if (this->isRegular) {
            return InstanceOf<TypeCheck, T *, std::is_polymorphic_v<
                                  std::remove_pointer_t<T>>>::getIfIsInstance(this->regular, res);
        }
        return InstanceOf<TypeCheck, T *, std::is_polymorphic_v<
                              std::remove_pointer_t<T>>>::get(this->smart.get(), res);
    }

    template<typename T>
    bool Pointer<T>::getIfPtrIsSmartPtr(Pointer::SmartPtrType &smartRef) const {
        if (!this->isRegular) {
            smartRef = this->smart;
            return true;
        }
        return false;
    }
}
