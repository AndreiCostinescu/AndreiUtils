//
// Created by Andrei on 18-Apr-24.
//

#pragma once

#include <AndreiUtils/traits/InstanceOf.hpp>
#include <memory>

namespace AndreiUtils {
    template<typename T>
    class Pointer {
    public:
        using ElementType = T;
        using SmartPtrType = std::shared_ptr<ElementType>;

        Pointer() : ptr(nullptr), smart(nullptr), isRegular(true) {}

        template<typename SubT>
        explicit Pointer(SubT datum) : ptr(nullptr), smart(nullptr), isRegular(true) {
            this->operator=(std::move(datum));
        }

        explicit Pointer(T &datum) : ptr(&datum), smart(), isRegular(true) {}

        explicit Pointer(T &&datum) : ptr(nullptr), smart(std::make_shared<T>(std::forward<T>(datum))),
                                      isRegular(false) {}

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T *datum) : ptr(datum), smart(nullptr), isRegular(true) {}  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        Pointer(SmartPtrType datum) :  // NOLINT(google-explicit-constructor)
                ptr(nullptr), smart(std::move(datum)), isRegular(false) {}

        Pointer(Pointer const &other) : ptr(other.ptr), smart(other.smart), isRegular(other.isRegular) {}

        Pointer(Pointer &&other) noexcept: ptr(other.ptr), smart(std::move(other.smart)), isRegular(other.isRegular) {
            other.ptr = nullptr;
            other.isRegular = false;
        }

        template<typename SubT>
        typename std::enable_if<std::is_base_of<T, SubT>::value, Pointer &>::type  // NOLINT(misc-unconventional-assign-operator)
        operator=(SubT other) {
            this->isRegular = false;
            this->ptr = nullptr;
            this->smart = std::make_shared<SubT>(std::move(other));
            return *this;
        }

        Pointer &operator=(T &other) {
            this->isRegular = true;
            this->ptr = &other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(T &&other) {
            this->isRegular = false;
            this->ptr = nullptr;
            this->smart = std::make_shared<T>(std::forward<T>(other));
            return *this;
        }

        Pointer &operator=(T *other) {
            this->isRegular = true;
            this->ptr = other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(Pointer const &other) {
            if (this != &other) {
                this->isRegular = other.isRegular;
                this->ptr = other.ptr;
                this->smart = other.smart;
            }
            return *this;
        }

        Pointer &operator=(Pointer &&other) noexcept {
            if (this != &other) {
                this->isRegular = other.isRegular;
                this->ptr = other.ptr;
                this->smart = std::move(other.smart);
                other.ptr = nullptr;
                other.isRegular = false;
            }
            return *this;
        }

        explicit operator bool() const { return this->get() != nullptr; }

        [[nodiscard]] T *get() const {
            return isRegular ? this->ptr : this->smart.get();
        }

        T &operator*() const noexcept {
            return *(this->get());
        }

        T *operator->() const noexcept {
            return this->get();
        }

        template<typename CastT>
        inline Pointer<CastT> constCast() const noexcept {
            if (this->isRegular) {
                return const_cast<CastT *>(this->ptr);
            }
            return std::const_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> constCastMove() && noexcept {
            if (this->isRegular) {
                Pointer<CastT> res(const_cast<CastT *>(this->ptr));
                this->ptr = nullptr;
                return res;
            }
            Pointer<CastT> res(std::make_shared<CastT>(std::move(*this->smart)));
            this->smart = nullptr;
            return res;
        }

        template<typename CastT>
        inline Pointer<CastT> dynamicCast() const noexcept {
            if (this->isRegular) {
                return dynamic_cast<CastT *>(this->ptr);
            }
            return std::dynamic_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> staticCast() const noexcept {
            if (this->isRegular) {
                return static_cast<CastT *>(this->ptr);
            }
            return std::static_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> reinterpretCast() const noexcept {
            if (this->isRegular) {
                return reinterpret_cast<CastT *>(this->ptr);
            }
            return std::reinterpret_pointer_cast<CastT>(this->smart);
        }

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf() const {
            if (this->isRegular) {
                return InstanceOf<TypeCheck, T *,
                        std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->ptr);
            } else {
                return InstanceOf<TypeCheck, T *,
                        std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->smart.get());
            }
        }

    protected:
        bool isRegular;
        T *ptr;
        SmartPtrType smart;
    };

    template<typename T>
    class Pointer<T const> {
    public:
        using ElementType = T const;
        using SmartPtrType = std::shared_ptr<ElementType>;

        Pointer() : ptr(nullptr), smart(nullptr), isRegular(true) {}

        template<typename SubT>
        explicit Pointer(SubT const &datum) : ptr(nullptr), smart(nullptr), isRegular(true) {
            this->operator=(std::move(datum));
        }

        explicit Pointer(T &datum) : ptr(&datum), smart(), isRegular(true) {}

        explicit Pointer(T const &datum) : ptr(&datum), smart(), isRegular(true) {}

        explicit Pointer(T &&datum) : ptr(nullptr), smart(std::make_shared<T const>(std::forward<T>(datum))),
                                      isRegular(false) {}

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T *datum) : ptr(datum), smart(nullptr), isRegular(true) {}  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from pointer to Pointer
        Pointer(T const *datum) : ptr(datum), smart(nullptr), isRegular(true) {}  // NOLINT(google-explicit-constructor)

        // no marking as explicit because we want the conversion from SmartPtrType to Pointer
        Pointer(SmartPtrType datum) :  // NOLINT(google-explicit-constructor)
                ptr(nullptr), smart(std::move(datum)), isRegular(false) {}

        Pointer(Pointer const &other) : ptr(other.ptr), smart(other.smart), isRegular(other.isRegular) {}

        Pointer(Pointer &&other) noexcept: ptr(other.ptr), smart(std::move(other.smart)), isRegular(other.isRegular) {
            other.ptr = nullptr;
            other.isRegular = false;
        }

        template<typename SubT>
        typename std::enable_if<std::is_base_of<T, SubT>::value, Pointer &>::type  // NOLINT(misc-unconventional-assign-operator)
        operator=(SubT const &other) {
            this->isRegular = true;
            this->ptr = &other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(T &other) {
            this->isRegular = true;
            this->ptr = &other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(T const &other) {
            this->isRegular = true;
            this->ptr = &other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(T &&other) {
            this->isRegular = false;
            this->ptr = nullptr;
            this->smart = std::make_shared<T const>(std::forward<T>(other));
            return *this;
        }

        Pointer &operator=(T *other) {
            this->isRegular = true;
            this->ptr = other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(T const *other) {
            this->isRegular = true;
            this->ptr = other;
            this->smart = nullptr;
            return *this;
        }

        Pointer &operator=(Pointer const &other) {
            if (this != &other) {
                this->isRegular = other.isRegular;
                this->ptr = other.ptr;
                this->smart = other.smart;
            }
            return *this;
        }

        Pointer &operator=(Pointer &&other) noexcept {
            if (this != &other) {
                this->isRegular = other.isRegular;
                this->ptr = other.ptr;
                this->smart = std::move(other.smart);
                other.ptr = nullptr;
                other.isRegular = false;
            }
            return *this;
        }

        Pointer &operator=(Pointer<T> const &other) {
            *this = other.template constCast<T const>();
            return *this;
        }

        Pointer &operator=(Pointer<T> &&other) noexcept {
            *this = std::move(std::move(other).template constCastMove<T const>());
            return *this;
        }

        explicit operator bool() const { return this->get() != nullptr; }

        [[nodiscard]] T const *get() const {
            return isRegular ? this->ptr : this->smart.get();
        }

        T const &operator*() const noexcept {
            return *(this->get());
        }

        T const *operator->() const noexcept {
            return this->get();
        }

        template<typename CastT>
        inline Pointer<CastT> constCast() const noexcept {
            if (this->isRegular) {
                return const_cast<CastT *>(this->ptr);
            }
            return std::const_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> dynamicCast() const noexcept {
            if (this->isRegular) {
                return dynamic_cast<CastT *>(this->ptr);
            }
            return std::dynamic_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> staticCast() const noexcept {
            if (this->isRegular) {
                return static_cast<CastT *>(this->ptr);
            }
            return std::static_pointer_cast<CastT>(this->smart);
        }

        template<typename CastT>
        inline Pointer<CastT> reinterpretCast() const noexcept {
            if (this->isRegular) {
                return reinterpret_cast<CastT *>(this->ptr);
            }
            return std::reinterpret_pointer_cast<CastT>(this->smart);
        }

        template<typename TypeCheck>
        [[nodiscard]] bool isInstanceOf() const {
            if (this->isRegular) {
                return InstanceOf<TypeCheck, T *,
                        std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->ptr);
            } else {
                return InstanceOf<TypeCheck, T *,
                        std::is_polymorphic<typename std::remove_pointer<T>::type>::value>::get(this->smart.get());
            }
        }

    protected:
        bool isRegular;
        T const *ptr;
        SmartPtrType smart;
    };

    template<typename T1, typename T2>
    inline bool
    operator==(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
        return ptr1.get() == ptr2.get();
    }

    template<typename T>
    inline bool operator==(AndreiUtils::Pointer<T> const &ptr, std::nullptr_t) noexcept { return !ptr; }

    template<typename T>
    inline bool operator==(std::nullptr_t, AndreiUtils::Pointer<T> const &ptr) noexcept { return !ptr; }

    template<typename T1, typename T2>
    inline bool
    operator!=(AndreiUtils::Pointer<T1> const &ptr1, AndreiUtils::Pointer<T2> const &ptr2) noexcept {
        return ptr1.get() != ptr2.get();
    }

    template<typename T>
    inline bool operator!=(AndreiUtils::Pointer<T> const &ptr, std::nullptr_t) noexcept { return (bool) ptr; }

    template<typename T>
    inline bool operator!=(std::nullptr_t, AndreiUtils::Pointer<T> const &ptr) noexcept { return (bool) ptr; }

    template<typename CastT, typename T>
    Pointer<CastT> static_pointer_cast(Pointer<T> const &p) {
        return p.template staticCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> const_pointer_cast(Pointer<T> const &p) {
        return p.template constCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> dynamic_pointer_cast(Pointer<T> const &p) {
        return p.template dynamicCast<CastT>();
    }

    template<typename CastT, typename T>
    Pointer<CastT> reinterpret_pointer_cast(Pointer<T> const &p) noexcept {
        return p.template reinterpretCast<CastT>();
    }

    template<typename TypeCheck, typename InstanceType>
    bool pointerInstanceOf(Pointer<InstanceType> const &val) {
        return val.template isInstanceOf<TypeCheck>();
    }
}
