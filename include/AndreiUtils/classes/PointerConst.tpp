//
// Created by Andrei on 03.11.24.
//

#include <AndreiUtils/classes/Pointer.hpp>

namespace AndreiUtils {
    template<typename T>
    Pointer<T const>::Pointer() : ptr(nullptr), smart(nullptr), isRegular(true) {}

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T const>::Pointer(std::nullptr_t) : Pointer() {}  // NOLINT(*-explicit-constructor)

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const>::Pointer(Type &datum) : ptr(&datum), smart(), isRegular(true) {}

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const>::Pointer(Type const &datum) : ptr(&datum), smart(), isRegular(true) {}

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const>::Pointer(Type &&datum) :
            ptr(nullptr), smart(std::make_shared<Type const>(std::forward<Type>(datum))), isRegular(false) {}

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T const>::Pointer(T *datum) : ptr(datum), smart(nullptr),
                                          isRegular(true) {}  // NOLINT(*-explicit-constructor)

    // no marking as explicit because we want the conversion from pointer to Pointer
    template<typename T>
    Pointer<T const>::Pointer(T const *datum) : ptr(datum), smart(nullptr),
                                                isRegular(true) {}  // NOLINT(*-explicit-constructor)

    // no marking as explicit because we want the conversion from SmartPtrType to Pointer
    template<typename T>
    Pointer<T const>::Pointer(SmartPtrType datum) :  // NOLINT(google-explicit-constructor)
            ptr(nullptr), smart(std::move(datum)), isRegular(false) {}

    template<typename T>
    Pointer<T const>::Pointer(Pointer const &other) : ptr(other.ptr), smart(other.smart), isRegular(other.isRegular) {}

    template<typename T>
    Pointer<T const>::Pointer(Pointer &&other) noexcept: ptr(other.ptr), smart(std::move(other.smart)),
                                                         isRegular(other.isRegular) {
        other.ptr = nullptr;
        other.isRegular = true;
    }

    template<typename T>
    template<typename Type>
    requires StrictSubTypeOfT<T, Type>
    Pointer<T const>::Pointer(Pointer<Type const> const &other) : Pointer(other.template cast<T>()) {}

    template<typename T>
    template<typename Type>
    requires StrictSubTypeOfT<T, Type>
    Pointer<T const>::Pointer(Pointer<Type const> &&other) noexcept:
            Pointer(std::move(std::move(other).template castMove<T>())) {}

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const>::Pointer(Pointer<Type> const &other) :   // NOLINT(*-explicit-constructor)
            Pointer(other.constCast()) {}

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const>::Pointer(Pointer<Type> &&other) noexcept:  // NOLINT(*-explicit-constructor)
            Pointer(std::move(std::move(other).constCastMove())) {}

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Type &other) {
        this->isRegular = true;
        this->ptr = &other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Type const &other) {
        this->isRegular = true;
        this->ptr = &other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Type &&other) {
        this->isRegular = false;
        this->ptr = nullptr;
        this->smart = std::make_shared<Type const>(std::forward<Type>(other));
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(std::nullptr_t) {
        this->isRegular = true;
        this->ptr = nullptr;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(T *other) {
        this->isRegular = true;
        this->ptr = other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(T const *other) {
        this->isRegular = true;
        this->ptr = other;
        this->smart = nullptr;
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(SmartPtrType other) {
        this->isRegular = false;
        this->ptr = nullptr;
        this->smart = std::move(other);
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(Pointer const &other) {
        if (this != &other) {
            this->isRegular = other.isRegular;
            this->ptr = other.ptr;
            this->smart = other.smart;
        }
        return *this;
    }

    template<typename T>
    Pointer<T const> &Pointer<T const>::operator=(Pointer &&other) noexcept {
        if (this != &other) {
            this->isRegular = other.isRegular;
            this->ptr = other.ptr;
            this->smart = std::move(other.smart);
            other.ptr = nullptr;
            other.isRegular = false;
        }
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires StrictSubTypeOfT<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Pointer<Type const> const &other) {
        *this = other.template cast<T const>();
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires StrictSubTypeOfT<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Pointer<Type const> &&other) noexcept {
        *this = std::move(std::move(other).template castMove<T const>());
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Pointer<Type> const &other) {
        *this = other.constCast();
        return *this;
    }

    template<typename T>
    template<typename Type>
    requires TypeWithSubTypes<T, Type>
    Pointer<T const> &Pointer<T const>::operator=(Pointer<Type> &&other) noexcept {
        *this = std::move(std::move(other).constCastMove());
        return *this;
    }

    template<typename T>
    bool Pointer<T const>::operator<(Pointer<T> const &other) const {
        return this->get() < other.get();
    }

    template<typename T>
    bool Pointer<T const>::operator<(Pointer<T const> const &other) const {
        return this->get() < other.get();
    }

    template<typename T>
    Pointer<T const>::operator bool() const { return this->get() != nullptr; }

    template<typename T>
    T const *Pointer<T const>::get() const {
        return isRegular ? this->ptr : this->smart.get();
    }

    template<typename T>
    T const &Pointer<T const>::operator*() const noexcept {
        return *(this->get());
    }

    template<typename T>
    T const *Pointer<T const>::operator->() const noexcept {
        return this->get();
    }

    template<typename T>
    void Pointer<T const>::reset() {
        this->isRegular = true;
        this->ptr = nullptr;
        this->smart = nullptr;
    }

    template<typename T>
    template<typename ParentCastT>
    requires TypeWithSubTypes<ParentCastT, T>
    Pointer<ParentCastT const> Pointer<T const>::cast() const noexcept {
        static_assert(std::is_base_of<ParentCastT, T>::value, "Can't cast to non-parent type!");
        if (this->isRegular) {
            return Pointer<ParentCastT const>(this->ptr);
        }
        return Pointer<ParentCastT const>(this->smart);
    }

    template<typename T>
    template<typename ParentCastT>
    requires TypeWithSubTypes<ParentCastT, T>
    Pointer<ParentCastT const> Pointer<T const>::castMove() && noexcept {
        static_assert(std::is_base_of<ParentCastT, T>::value, "Can't cast to non-parent type!");
        if (this->isRegular) {
            Pointer<ParentCastT const> res(this->ptr);
            this->ptr = nullptr;
            return res;
        }
        Pointer<ParentCastT const> res(std::move(this->smart));
        this->isRegular = true;
        return res;
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T const>::constCast() const noexcept {
        if (this->isRegular) {
            return const_cast<CastT *>(this->ptr);
        }
        return std::const_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T const>::dynamicCast() const noexcept {
        if (this->isRegular) {
            return dynamic_cast<CastT *>(this->ptr);
        }
        return std::dynamic_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T const>::staticCast() const noexcept {
        if (this->isRegular) {
            return static_cast<CastT *>(this->ptr);
        }
        return std::static_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename CastT>
    Pointer<CastT> Pointer<T const>::reinterpretCast() const noexcept {
        if (this->isRegular) {
            return reinterpret_cast<CastT *>(this->ptr);
        }
        return std::reinterpret_pointer_cast<CastT>(this->smart);
    }

    template<typename T>
    template<typename TypeCheck>
    bool Pointer<T const>::isInstanceOf() const {
        if (this->isRegular) {
            return InstanceOf<TypeCheck, T *,
                    std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->ptr);
        } else {
            return InstanceOf<TypeCheck, T *,
                    std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->smart.get());
        }
    }
}
